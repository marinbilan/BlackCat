#include "PidBase.h"

namespace LabSys::Modules::Pid {

PidBase::PidBase(std::string name) : m_name(std::move(name)) {}

const std::string& PidBase::instanceName() const { return m_name; }

void PidBase::createChannels()
{
    // Convention: OutputChannel.<name>.Control
    // Registered with RM via NATS labsys.registry.channel.create
    // in PidMaster::registerChannelsWithRM().
    // TODO: create SpscChannel shm segment here (shm_open / mmap)
}

void PidBase::attachInputChannels()
{
    // Setpoint and Measurement shm paths resolved via NATS
    // labsys.registry.channel.get.<name> in PidMaster::attachInputChannels().
    // TODO: open returned shm_paths and attach SpscChannel input ends
}

void PidBase::start() {}
void PidBase::stop()  {}

void PidBase::tick()
{
    // TODO: read setpoint + measurement from input channels
    // double setpoint    = readInputChannel("Setpoint");
    // double measurement = readInputChannel("Measurement");
    // double error  = setpoint - measurement;
    // m_integral   += error;
    // double deriv  = error - m_prevError;
    // m_prevError   = error;
    // double output = m_kp * error + m_ki * m_integral + m_kd * deriv;
    // TODO: write output to output channel
}

} // namespace LabSys::Modules::Pid
