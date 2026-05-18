#include "ChannelRegistry.h"
#include "ChannelIf.h"

namespace LabSys::Registry {

ChannelRegistry& ChannelRegistry::instance()
{
    static ChannelRegistry s_instance;
    return s_instance;
}

bool ChannelRegistry::registerChannel(std::shared_ptr<Channel::ChannelIf> channel)
{
    std::lock_guard lock(m_mutex);
    auto [_, inserted] = m_channels.emplace(channel->name(), std::move(channel));
    return inserted;
}

std::shared_ptr<Channel::ChannelIf> ChannelRegistry::getChannel(const std::string& name) const
{
    std::lock_guard lock(m_mutex);
    auto it = m_channels.find(name);
    return (it != m_channels.end()) ? it->second : nullptr;
}

void ChannelRegistry::unregisterChannel(const std::string& name)
{
    std::lock_guard lock(m_mutex);
    m_channels.erase(name);
}

} // namespace LabSys::Registry
