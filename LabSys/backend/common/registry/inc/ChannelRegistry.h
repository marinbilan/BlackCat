#pragma once
#include "ChannelRegistryIf.h"
#include <unordered_map>
#include <mutex>

namespace LabSys::Registry {

// ChannelRegistry — thread-safe singleton implementation.
//
// DEPRECATED (in-process singleton).
// This class is no longer compiled into LabSysCommon.  The Runtime Manager
// owns the Channel Registry internally (RuntimeManager::m_channelRegistry)
// and exposes it cross-process via NATS (labsys.registry.channel.*).
// This file is retained for reference only.
class ChannelRegistry : public ChannelRegistryIf
{
public:
    // Singleton accessor
    static ChannelRegistry& instance();

    // Non-copyable / non-movable
    ChannelRegistry(const ChannelRegistry&)            = delete;
    ChannelRegistry& operator=(const ChannelRegistry&) = delete;

    bool registerChannel(std::shared_ptr<Channel::ChannelIf> channel) override;
    std::shared_ptr<Channel::ChannelIf> getChannel(const std::string& name) const override;
    void unregisterChannel(const std::string& name) override;

private:
    ChannelRegistry() = default;

    mutable std::mutex                                           m_mutex;
    std::unordered_map<std::string, std::shared_ptr<Channel::ChannelIf>> m_channels;
};

} // namespace LabSys::Registry
