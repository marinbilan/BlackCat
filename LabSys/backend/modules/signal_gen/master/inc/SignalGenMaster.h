#pragma once
#include "SignalGenMasterIf.h"
#include "SgBaseIf.h"
#include <nats/nats.h>
#include <string>
#include <vector>
#include <memory>
#include <mutex>

namespace LabSys::Modules::SignalGen {

class SignalGenMaster : public SignalGenMasterIf
{
public:
    explicit SignalGenMaster(const std::string& natsUrl    = "nats://localhost:4222",
                             const std::string& moduleName = "mod_signal_gen");
    ~SignalGenMaster() override;

    void onCreate() override;
    void onInit()                   override;
    void onCreateChannels()         override;
    void onAttach()                 override;
    void run()                      override;

private:
    void connectNats();
    void subscribeLifecycle();

    // Register all created devices with the Runtime Manager Device Registry
    void registerDevicesWithRM();

    // Register all output channels with the Runtime Manager Channel Registry
    void registerChannelsWithRM();

    // Attach input channels by querying the RM Channel Registry
    void attachInputChannels();

    // Static NATS lifecycle callbacks
    static void onLifecycleCreate        (natsConnection*, natsSubscription*, natsMsg*, void*);
    static void onLifecycleInit          (natsConnection*, natsSubscription*, natsMsg*, void*);
    static void onLifecycleCreateChannels(natsConnection*, natsSubscription*, natsMsg*, void*);
    static void onLifecycleAttach        (natsConnection*, natsSubscription*, natsMsg*, void*);

    std::string m_natsUrl;
    std::string m_moduleName;

    natsConnection*   m_natsConn            {nullptr};
    natsSubscription* m_subCreate           {nullptr};
    natsSubscription* m_subInit             {nullptr};
    natsSubscription* m_subCreateChannels   {nullptr};
    natsSubscription* m_subAttach           {nullptr};

    mutable std::mutex                           m_mutex;
    std::vector<std::shared_ptr<SgBaseIf>>       m_devices;
};

} // namespace LabSys::Modules::SignalGen
