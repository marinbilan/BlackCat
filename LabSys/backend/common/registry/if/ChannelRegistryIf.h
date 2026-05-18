#pragma once
#include <string>
#include <memory>

// Forward declarations only — keep this header dependency-free
namespace LabSys::Channel { class ChannelIf; }

namespace LabSys::Registry {

// ChannelRegistryIf — pure virtual interface for the Channel Registry.
//
// DEPRECATED (in-process singleton).
// The Channel Registry is now owned by the Runtime Manager and exposed
// cross-process via NATS (labsys.registry.channel.*).
// Device modules must NOT use this interface directly; they interact with
// the registry through their Master class via NATS request/reply.
// This file is kept for reference only and is not compiled into LabSysCommon.
class ChannelRegistryIf
{
public:
    virtual ~ChannelRegistryIf() = default;

    // Register an output channel. Called by each device during CreateChannels phase.
    // Returns false if a channel with that name already exists.
    virtual bool registerChannel(std::shared_ptr<Channel::ChannelIf> channel) = 0;

    // Look up a channel by name. Returns nullptr if not found.
    virtual std::shared_ptr<Channel::ChannelIf> getChannel(const std::string& name) const = 0;

    // Remove a channel (called on device teardown).
    virtual void unregisterChannel(const std::string& name) = 0;
};

} // namespace LabSys::Registry
