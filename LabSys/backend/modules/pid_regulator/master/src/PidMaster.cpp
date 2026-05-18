#include "PidMaster.h"
#include "PidBaseIf.h"
#include "PidBase.h"
#include "Log.h"
#include <nlohmann/json.hpp>
#include <stdexcept>
#include <unistd.h>   // getpid()

using json = nlohmann::json;

namespace LabSys::Modules::Pid {

static constexpr int64_t NATS_REQ_TIMEOUT_MS = 2000;

// ── Construction / Destruction ────────────────────────────────────────────────

PidMaster::PidMaster(const std::string& natsUrl, const std::string& moduleName)
    : m_natsUrl(natsUrl), m_moduleName(moduleName)
{}

PidMaster::~PidMaster()
{
    natsSubscription_Destroy(m_subCreate);
    natsSubscription_Destroy(m_subInit);
    natsSubscription_Destroy(m_subCreateChannels);
    natsSubscription_Destroy(m_subAttach);
    natsConnection_Destroy(m_natsConn);
}

// ── NATS setup ────────────────────────────────────────────────────────────────

void PidMaster::connectNats()
{
    natsStatus s = natsConnection_ConnectTo(&m_natsConn, m_natsUrl.c_str());
    if (s != NATS_OK)
        throw std::runtime_error(std::string("[PidMaster] NATS connect failed: ")
                                 + natsStatus_GetText(s));
    LOG << "[PidMaster] NATS connected to " << m_natsUrl;
}

void PidMaster::subscribeLifecycle()
{
    auto sub = [&](natsSubscription** dest, const char* subj, natsMsgHandler cb) {
        natsStatus s = natsConnection_Subscribe(dest, m_natsConn, subj, cb, this);
        if (s != NATS_OK)
            throw std::runtime_error(std::string("[PidMaster] subscribe failed: ") + subj);
    };

    sub(&m_subCreate,         "labsys.lifecycle.create",         onLifecycleCreate);
    sub(&m_subInit,           "labsys.lifecycle.init",           onLifecycleInit);
    sub(&m_subCreateChannels, "labsys.lifecycle.createChannels", onLifecycleCreateChannels);
    sub(&m_subAttach,         "labsys.lifecycle.attach",         onLifecycleAttach);

    LOG << "[PidMaster] Subscribed to lifecycle subjects";
}

// ── Lifecycle implementation ──────────────────────────────────────────────────

void PidMaster::onCreate()
{
    std::lock_guard<std::mutex> lock(m_mutex);

    m_devices.push_back(std::make_shared<PidBase>("pid_0"));
    m_devices.push_back(std::make_shared<PidBase>("pid_1"));

    for (auto& dev : m_devices)
        LOG << "[PidMaster] Created device: " << dev->instanceName();

    registerDevicesWithRM();
}

void PidMaster::onInit()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    LOG << "[PidMaster] onInit — applying config params to devices";
    for (auto& dev : m_devices) {
        // TODO: push kp, ki, kd gains from JSON config
        (void)dev;
    }
}

void PidMaster::onCreateChannels()
{
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        for (auto& dev : m_devices)
            dev->createChannels();
    }
    registerChannelsWithRM();
}

void PidMaster::onAttach()
{
    attachInputChannels();
    std::lock_guard<std::mutex> lock(m_mutex);
    for (auto& dev : m_devices)
        dev->attachInputChannels();
}

// ── Registry helpers ──────────────────────────────────────────────────────────

void PidMaster::registerDevicesWithRM()
{
    // Called with m_mutex already held
    for (auto& dev : m_devices) {
        json payload{
            {"type",         "PidBase"},
            {"instanceName", dev->instanceName()},
            {"moduleName",   m_moduleName},
            {"pid",          static_cast<int>(::getpid())}
        };

        natsMsg* reply = nullptr;
        natsStatus s = natsConnection_Request(&reply, m_natsConn,
                "labsys.registry.device.register",
                payload.dump().c_str(),
                static_cast<int>(payload.dump().size()),
                NATS_REQ_TIMEOUT_MS);

        if (s == NATS_OK) {
            LOG << "[PidMaster] Device registered: " << dev->instanceName();
            natsMsg_Destroy(reply);
        } else {
            LOGE << "[PidMaster] Device register failed for "
                 << dev->instanceName() << ": " << natsStatus_GetText(s);
        }
    }
}

void PidMaster::registerChannelsWithRM()
{
    std::lock_guard<std::mutex> lock(m_mutex);

    // PID output: control signal
    for (auto& dev : m_devices) {
        std::string chanName = "OutputChannel." + dev->instanceName() + ".Control";
        std::string shmPath  = "/labsys_" + dev->instanceName() + "_ctrl";

        json payload{
            {"name",      chanName},
            {"shm_path",  shmPath},
            {"capacity",  1024}
        };

        natsMsg* reply = nullptr;
        natsStatus s = natsConnection_Request(&reply, m_natsConn,
                "labsys.registry.channel.create",
                payload.dump().c_str(),
                static_cast<int>(payload.dump().size()),
                NATS_REQ_TIMEOUT_MS);

        if (s == NATS_OK) {
            LOG << "[PidMaster] Channel registered: " << chanName;
            natsMsg_Destroy(reply);
        } else {
            LOGE << "[PidMaster] Channel register failed for "
                 << chanName << ": " << natsStatus_GetText(s);
        }
    }
}

void PidMaster::attachInputChannels()
{
    // PID needs: Setpoint and Measurement input channels.
    // These are produced by other devices (e.g. signal_gen for setpoint).
    //
    // Example lookup:
    //   std::string subject = "labsys.registry.channel.get.OutputChannel.mod_signal_gen_sg_0.Ch0";
    //   natsMsg* reply = nullptr;
    //   natsStatus s = natsConnection_Request(&reply, m_natsConn,
    //           subject.c_str(), "", 0, NATS_REQ_TIMEOUT_MS);
    //   if (s == NATS_OK) {
    //       auto j = json::parse(natsMsg_GetData(reply),
    //                            natsMsg_GetData(reply) + natsMsg_GetDataLength(reply));
    //       auto shmPath  = j.at("shm_path").get<std::string>();
    //       auto capacity = j.at("capacity").get<std::size_t>();
    //       // TODO: open shmPath and attach SpscChannel input end for Setpoint
    //       natsMsg_Destroy(reply);
    //   }

    LOG << "[PidMaster] onAttach — TODO: attach Setpoint and Measurement input channels";
}

// ── Static NATS callbacks ─────────────────────────────────────────────────────

void PidMaster::onLifecycleCreate(natsConnection*, natsSubscription*,
                                   natsMsg* msg, void* ctx)
{
    auto* self = static_cast<PidMaster*>(ctx);
    LOG << "[PidMaster] Lifecycle: CREATE received";
    self->onCreate();
    natsMsg_Destroy(msg);
}

void PidMaster::onLifecycleInit(natsConnection*, natsSubscription*,
                                 natsMsg* msg, void* ctx)
{
    auto* self = static_cast<PidMaster*>(ctx);
    LOG << "[PidMaster] Lifecycle: INIT received";
    self->onInit();
    natsMsg_Destroy(msg);
}

void PidMaster::onLifecycleCreateChannels(natsConnection*, natsSubscription*,
                                           natsMsg* msg, void* ctx)
{
    auto* self = static_cast<PidMaster*>(ctx);
    LOG << "[PidMaster] Lifecycle: CREATE_CHANNELS received";
    self->onCreateChannels();
    natsMsg_Destroy(msg);
}

void PidMaster::onLifecycleAttach(natsConnection*, natsSubscription*,
                                   natsMsg* msg, void* ctx)
{
    auto* self = static_cast<PidMaster*>(ctx);
    LOG << "[PidMaster] Lifecycle: ATTACH received";
    self->onAttach();
    natsMsg_Destroy(msg);
}

// ── run() ─────────────────────────────────────────────────────────────────────

void PidMaster::run()
{
    connectNats();
    subscribeLifecycle();

    LOG << "[PidMaster] Listening for lifecycle events on NATS...";
    while (true) {
        nats_Sleep(1000);
    }
}

} // namespace LabSys::Modules::Pid
