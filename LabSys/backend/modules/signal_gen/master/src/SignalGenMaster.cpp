#include "SignalGenMaster.h"
#include "SgBaseIf.h"
#include "SgVendorA.h"
#include "Log.h"
#include <nlohmann/json.hpp>
#include <stdexcept>
#include <unistd.h>   // getpid()

using json = nlohmann::json;

namespace LabSys::Modules::SignalGen {

// Timeout for NATS request/reply with the Runtime Manager (milliseconds)
static constexpr int64_t NATS_REQ_TIMEOUT_MS = 2000;

// ── Construction / Destruction ────────────────────────────────────────────────

SignalGenMaster::SignalGenMaster(const std::string& natsUrl,
                                 const std::string& moduleName)
    : m_natsUrl(natsUrl), m_moduleName(moduleName)
{}

SignalGenMaster::~SignalGenMaster()
{
    natsSubscription_Destroy(m_subCreate);
    natsSubscription_Destroy(m_subInit);
    natsSubscription_Destroy(m_subCreateChannels);
    natsSubscription_Destroy(m_subAttach);
    natsConnection_Destroy(m_natsConn);
}

// ── NATS setup ────────────────────────────────────────────────────────────────

void SignalGenMaster::connectNats()
{
    natsStatus s = natsConnection_ConnectTo(&m_natsConn, m_natsUrl.c_str());
    if (s != NATS_OK)
        throw std::runtime_error(std::string("[SignalGenMaster] NATS connect failed: ")
                                 + natsStatus_GetText(s));
    LOG << "[SignalGenMaster] NATS connected to " << m_natsUrl;
}

void SignalGenMaster::subscribeLifecycle()
{
    auto sub = [&](natsSubscription** dest, const char* subj, natsMsgHandler cb) {
        natsStatus s = natsConnection_Subscribe(dest, m_natsConn, subj, cb, this);
        if (s != NATS_OK)
            throw std::runtime_error(std::string("[SignalGenMaster] subscribe failed: ") + subj);
    };

    sub(&m_subCreate,         "labsys.lifecycle.create",         onLifecycleCreate);
    sub(&m_subInit,           "labsys.lifecycle.init",           onLifecycleInit);
    sub(&m_subCreateChannels, "labsys.lifecycle.createChannels", onLifecycleCreateChannels);
    sub(&m_subAttach,         "labsys.lifecycle.attach",         onLifecycleAttach);

    LOG << "[SignalGenMaster] Subscribed to lifecycle subjects";
}

// ── Lifecycle implementation ──────────────────────────────────────────────────

void SignalGenMaster::onCreate()
{
    std::lock_guard<std::mutex> lock(m_mutex);

    m_devices.push_back(std::make_shared<SgVendorA>("sg_0"));
    m_devices.push_back(std::make_shared<SgVendorA>("sg_1"));

    for (auto& dev : m_devices)
        LOG << "[SignalGenMaster] Created device: " << dev->instanceName();

    registerDevicesWithRM();
}

void SignalGenMaster::onInit()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    LOG << "[SignalGenMaster] onInit — applying config params to devices";
    for (auto& dev : m_devices) {
        // TODO: push config params (frequency, amplitude, …) from JSON config
        (void)dev;
    }
}

void SignalGenMaster::onCreateChannels()
{
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        for (auto& dev : m_devices)
            dev->createChannels();   // device creates its SpscChannel(s)
    }
    registerChannelsWithRM();
}

void SignalGenMaster::onAttach()
{
    attachInputChannels();
    std::lock_guard<std::mutex> lock(m_mutex);
    for (auto& dev : m_devices)
        dev->attachInputChannels();
}

// ── Registry helpers ──────────────────────────────────────────────────────────

void SignalGenMaster::registerDevicesWithRM()
{
    // Called with m_mutex already held
    for (auto& dev : m_devices) {
        json payload{
            {"type",         "SgVendorA"},
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
            LOG << "[SignalGenMaster] Device registered: " << dev->instanceName();
            natsMsg_Destroy(reply);
        } else {
            LOGE << "[SignalGenMaster] Device register request failed for "
                 << dev->instanceName() << ": " << natsStatus_GetText(s);
        }
    }
}

void SignalGenMaster::registerChannelsWithRM()
{
    std::lock_guard<std::mutex> lock(m_mutex);

    // Each SG device owns one output channel per channel index.
    // Convention: OutputChannel.<instanceName>.Ch<N>
    static constexpr int NUM_OUTPUT_CHANNELS = 1;

    for (auto& dev : m_devices) {
        for (int ch = 0; ch < NUM_OUTPUT_CHANNELS; ++ch) {
            std::string chanName = "OutputChannel." + dev->instanceName()
                                   + ".Ch" + std::to_string(ch);
            std::string shmPath  = "/labsys_" + dev->instanceName()
                                   + "_ch" + std::to_string(ch);

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
                LOG << "[SignalGenMaster] Channel registered: " << chanName;
                natsMsg_Destroy(reply);
            } else {
                LOGE << "[SignalGenMaster] Channel register failed for "
                     << chanName << ": " << natsStatus_GetText(s);
            }
        }
    }
}

void SignalGenMaster::attachInputChannels()
{
    // Signal generators typically consume no input channels.
    // Example lookup shown here for reference.
    //
    // std::string subject = "labsys.registry.channel.get.OutputChannel.SomeDevice.Ch0";
    // natsMsg* reply = nullptr;
    // natsStatus s = natsConnection_Request(&reply, m_natsConn,
    //         subject.c_str(), "", 0, NATS_REQ_TIMEOUT_MS);
    // if (s == NATS_OK) {
    //     auto j = json::parse(natsMsg_GetData(reply), natsMsg_GetData(reply)
    //                          + natsMsg_GetDataLength(reply));
    //     auto shmPath  = j.at("shm_path").get<std::string>();
    //     auto capacity = j.at("capacity").get<std::size_t>();
    //     // TODO: open shared memory and attach SpscChannel input end
    //     natsMsg_Destroy(reply);
    // }
    LOG << "[SignalGenMaster] onAttach — no input channels needed";
}

// ── Static NATS callbacks ─────────────────────────────────────────────────────

void SignalGenMaster::onLifecycleCreate(natsConnection*, natsSubscription*,
                                         natsMsg* msg, void* ctx)
{
    auto* self = static_cast<SignalGenMaster*>(ctx);
    LOG << "[SignalGenMaster] Lifecycle: CREATE received";
    self->onCreate();
    natsMsg_Destroy(msg);
}

void SignalGenMaster::onLifecycleInit(natsConnection*, natsSubscription*,
                                       natsMsg* msg, void* ctx)
{
    auto* self = static_cast<SignalGenMaster*>(ctx);
    LOG << "[SignalGenMaster] Lifecycle: INIT received";
    self->onInit();
    natsMsg_Destroy(msg);
}

void SignalGenMaster::onLifecycleCreateChannels(natsConnection*, natsSubscription*,
                                                  natsMsg* msg, void* ctx)
{
    auto* self = static_cast<SignalGenMaster*>(ctx);
    LOG << "[SignalGenMaster] Lifecycle: CREATE_CHANNELS received";
    self->onCreateChannels();
    natsMsg_Destroy(msg);
}

void SignalGenMaster::onLifecycleAttach(natsConnection*, natsSubscription*,
                                         natsMsg* msg, void* ctx)
{
    auto* self = static_cast<SignalGenMaster*>(ctx);
    LOG << "[SignalGenMaster] Lifecycle: ATTACH received";
    self->onAttach();
    natsMsg_Destroy(msg);
}

// ── run() ─────────────────────────────────────────────────────────────────────

void SignalGenMaster::run()
{
    connectNats();
    subscribeLifecycle();

    LOG << "[SignalGenMaster] Listening for lifecycle events on NATS...";
    // NATS delivers messages on its internal thread; main thread keeps process alive.
    while (true) {
        nats_Sleep(1000);
    }
}

} // namespace LabSys::Modules::SignalGen
