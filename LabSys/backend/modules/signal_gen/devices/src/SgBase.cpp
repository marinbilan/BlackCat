#include "SgBase.h"
#include "SpscChannel.h"

namespace LabSys::Modules::SignalGen {

SgBase::SgBase(std::string name) : m_name(std::move(name)) {}

const std::string& SgBase::instanceName() const { return m_name; }

void SgBase::createChannels()
{
    // Convention: OutputChannel.<DeviceName>.Ch<N>
    // Channels are registered with the Runtime Manager via NATS request
    // labsys.registry.channel.create — see SignalGenMaster::registerChannelsWithRM().
    // TODO: create SpscChannel shm segments here (shm_open / mmap)
}

void SgBase::attachInputChannels()
{
    // Input channel shm paths are resolved by querying the RM via NATS
    // labsys.registry.channel.get.<name> — see SignalGenMaster::attachInputChannels().
    // TODO: open returned shm_path and attach SpscChannel input end
}

void SgBase::start() { /* TODO */ }
void SgBase::stop()  { /* TODO */ }
void SgBase::tick()  { /* TODO: generate and push sample to output channel */ }

} // namespace LabSys::Modules::SignalGen
