#include "RuntimeManager.h"
#include "Log.h"
#include <nlohmann/json.hpp>
#include <stdexcept>
#include <string>

using json = nlohmann::json;

namespace LabSys {

// ── Construction / Destruction ────────────────────────────────────────────────

RuntimeManager::RuntimeManager(const std::string& natsUrl)
    : m_natsUrl(natsUrl)
{
    connectNats();
    subscribeRegistryHandlers();
}

RuntimeManager::~RuntimeManager()
{
    natsSubscription_Destroy(m_subDeviceReg);
    natsSubscription_Destroy(m_subChannelCreate);
    natsSubscription_Destroy(m_subChannelGet);
    natsConnection_Destroy(m_natsConn);
}

// ── NATS setup ────────────────────────────────────────────────────────────────

void RuntimeManager::connectNats()
{
    natsStatus s = natsConnection_ConnectTo(&m_natsConn, m_natsUrl.c_str());
    if (s != NATS_OK)
        throw std::runtime_error(std::string("[RM] NATS connect failed: ") + natsStatus_GetText(s));
    LOG << "[RM] NATS connected to " << m_natsUrl;
}

void RuntimeManager::subscribeRegistryHandlers()
{
    natsStatus s;

    s = natsConnection_Subscribe(&m_subDeviceReg, m_natsConn,
            "labsys.registry.device.register", onDeviceRegister, this);
    if (s != NATS_OK)
        throw std::runtime_error("[RM] subscribe device.register failed");

    s = natsConnection_Subscribe(&m_subChannelCreate, m_natsConn,
            "labsys.registry.channel.create", onChannelCreate, this);
    if (s != NATS_OK)
        throw std::runtime_error("[RM] subscribe channel.create failed");

    // '>' matches one or more tokens — handles channel names with embedded dots
    // e.g. labsys.registry.channel.get.OutputChannel.Device0.Ch0
    s = natsConnection_Subscribe(&m_subChannelGet, m_natsConn,
            "labsys.registry.channel.get.>", onChannelGet, this);
    if (s != NATS_OK)
        throw std::runtime_error("[RM] subscribe channel.get.> failed");

    LOG << "[RM] Registry NATS handlers subscribed";
}

// ── NATS message handlers ─────────────────────────────────────────────────────

void RuntimeManager::onDeviceRegister(natsConnection* nc, natsSubscription*,
                                       natsMsg* msg, void* ctx)
{
    auto* self = static_cast<RuntimeManager*>(ctx);
    std::string payload(natsMsg_GetData(msg), static_cast<std::size_t>(natsMsg_GetDataLength(msg)));
    const char* replyTo = natsMsg_GetReply(msg);

    try {
        auto j = json::parse(payload);
        DeviceInfo info{
            j.at("type").get<std::string>(),
            j.at("instanceName").get<std::string>(),
            j.at("moduleName").get<std::string>(),
            j.at("pid").get<int>()
        };
        {
            std::lock_guard<std::mutex> lock(self->m_mutex);
            self->m_deviceRegistry[info.instanceName] = info;
        }
        LOG << "[RM] Device registered: " << info.instanceName
            << "  type=" << info.type
            << "  module=" << info.moduleName
            << "  pid=" << info.pid;

        if (replyTo)
            natsConnection_PublishString(nc, replyTo, R"({"status":"ok"})");
    }
    catch (const std::exception& e) {
        LOGE << "[RM] onDeviceRegister parse error: " << e.what();
        if (replyTo)
            natsConnection_PublishString(nc, replyTo, R"({"status":"error"})");
    }
    natsMsg_Destroy(msg);
}

void RuntimeManager::onChannelCreate(natsConnection* nc, natsSubscription*,
                                      natsMsg* msg, void* ctx)
{
    auto* self = static_cast<RuntimeManager*>(ctx);
    std::string payload(natsMsg_GetData(msg), static_cast<std::size_t>(natsMsg_GetDataLength(msg)));
    const char* replyTo = natsMsg_GetReply(msg);

    try {
        auto j = json::parse(payload);
        ChannelInfo info{
            j.at("name").get<std::string>(),
            j.at("shm_path").get<std::string>(),
            j.at("capacity").get<std::size_t>()
        };
        {
            std::lock_guard<std::mutex> lock(self->m_mutex);
            self->m_channelRegistry[info.name] = info;
        }
        LOG << "[RM] Channel registered: " << info.name
            << "  shm=" << info.shmPath
            << "  cap=" << info.capacity;

        if (replyTo)
            natsConnection_PublishString(nc, replyTo, R"({"status":"ok"})");
    }
    catch (const std::exception& e) {
        LOGE << "[RM] onChannelCreate parse error: " << e.what();
        if (replyTo)
            natsConnection_PublishString(nc, replyTo, R"({"status":"error"})");
    }
    natsMsg_Destroy(msg);
}

void RuntimeManager::onChannelGet(natsConnection* nc, natsSubscription*,
                                   natsMsg* msg, void* ctx)
{
    auto* self  = static_cast<RuntimeManager*>(ctx);
    const char* subject = natsMsg_GetSubject(msg);
    const char* replyTo = natsMsg_GetReply(msg);

    // Subject: labsys.registry.channel.get.<channelName>
    // Strip the fixed prefix to recover the channel name.
    static constexpr std::string_view PREFIX = "labsys.registry.channel.get.";
    std::string channelName;
    std::string_view subj(subject ? subject : "");
    if (subj.starts_with(PREFIX))
        channelName = std::string(subj.substr(PREFIX.size()));

    if (replyTo) {
        std::lock_guard<std::mutex> lock(self->m_mutex);
        auto it = self->m_channelRegistry.find(channelName);
        if (it != self->m_channelRegistry.end()) {
            json resp{
                {"shm_path", it->second.shmPath},
                {"capacity", it->second.capacity}
            };
            natsConnection_PublishString(nc, replyTo, resp.dump().c_str());
        } else {
            LOG << "[RM] Channel lookup miss: " << channelName;
            natsConnection_PublishString(nc, replyTo,
                R"({"status":"error","reason":"not found"})");
        }
    }
    natsMsg_Destroy(msg);
}

// ── Lifecycle phases ──────────────────────────────────────────────────────────

void RuntimeManager::onCreate()
{
    LOG << "[RM] ── Phase: CREATE ──";
    natsConnection_PublishString(m_natsConn, "labsys.lifecycle.create", "{}");
    natsConnection_Flush(m_natsConn);
    // Allow modules time to process create and register their devices
    nats_Sleep(500);
}

void RuntimeManager::onInit()
{
    LOG << "[RM] ── Phase: INIT ──";
    natsConnection_PublishString(m_natsConn, "labsys.lifecycle.init", "{}");
    natsConnection_Flush(m_natsConn);
    nats_Sleep(500);
}

void RuntimeManager::onCreateChannels()
{
    LOG << "[RM] ── Phase: CREATE_CHANNELS ──";
    natsConnection_PublishString(m_natsConn, "labsys.lifecycle.createChannels", "{}");
    natsConnection_Flush(m_natsConn);
    nats_Sleep(500);
}

void RuntimeManager::onAttach()
{
    LOG << "[RM] ── Phase: ATTACH ──";
    natsConnection_PublishString(m_natsConn, "labsys.lifecycle.attach", "{}");
    natsConnection_Flush(m_natsConn);
    nats_Sleep(500);
}

// ── Main loop ─────────────────────────────────────────────────────────────────

void RuntimeManager::run()
{
    // NOTE: start module processes before running RM so they can subscribe
    //       to lifecycle subjects before RM publishes them.
    onCreate();
    onInit();
    onCreateChannels();
    onAttach();

    LOG << "[RM] System up — entering event loop";
    // NATS callbacks dispatched on the internal delivery thread;
    // main thread just keeps the process alive.
    while (true) {
        nats_Sleep(1000);
    }
}

} // namespace LabSys
