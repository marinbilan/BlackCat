#pragma once
#include "RuntimeManagerIf.h"
#include <nats/nats.h>
#include <string>
#include <unordered_map>
#include <mutex>

namespace LabSys {

// ── Registry entry types ──────────────────────────────────────────────────────

struct DeviceInfo {
    std::string type;
    std::string instanceName;
    std::string moduleName;
    int         pid{0};
};

struct ChannelInfo {
    std::string name;
    std::string shmPath;
    std::size_t capacity{0};
};

// ── Runtime Manager ──────────────────────────────────────────────────────────
//
// Central orchestrator.  Owns both the Device Registry and Channel Registry
// (previously an in-process singleton — now exposed cross-process via NATS).
//
// NATS subject namespace
//   Lifecycle  (RM → modules, pub/sub):
//     labsys.lifecycle.create
//     labsys.lifecycle.init
//     labsys.lifecycle.createChannels
//     labsys.lifecycle.attach
//
//   Device registration  (module → RM, request/reply):
//     labsys.registry.device.register
//       payload: {"type","instanceName","moduleName","pid"}
//
//   Channel registration  (module → RM, request/reply):
//     labsys.registry.channel.create
//       payload: {"name","shm_path","capacity"}
//
//   Channel lookup  (module → RM, request/reply):
//     labsys.registry.channel.get.>
//       subject suffix IS the channel name
//       reply: {"shm_path","capacity"}  |  {"status":"error","reason":"not found"}

class RuntimeManager : public RuntimeManagerIf
{
public:
    explicit RuntimeManager(const std::string& natsUrl = "nats://localhost:4222");
    ~RuntimeManager() override;

    void onCreate()         override;
    void onInit()           override;
    void onCreateChannels() override;
    void onAttach()         override;
    void run()              override;

private:
    void connectNats();
    void subscribeRegistryHandlers();

    // Static NATS message handlers
    static void onDeviceRegister(natsConnection*, natsSubscription*, natsMsg*, void*);
    static void onChannelCreate (natsConnection*, natsSubscription*, natsMsg*, void*);
    static void onChannelGet    (natsConnection*, natsSubscription*, natsMsg*, void*);

    std::string       m_natsUrl;
    natsConnection*   m_natsConn         {nullptr};
    natsSubscription* m_subDeviceReg     {nullptr};
    natsSubscription* m_subChannelCreate {nullptr};
    natsSubscription* m_subChannelGet    {nullptr};

    mutable std::mutex m_mutex;

    // Device Registry: instanceName → DeviceInfo
    std::unordered_map<std::string, DeviceInfo>  m_deviceRegistry;

    // Channel Registry: channelName → ChannelInfo
    // Replaces the in-process ChannelRegistry singleton — exposed via NATS externally.
    std::unordered_map<std::string, ChannelInfo> m_channelRegistry;
};

} // namespace LabSys
