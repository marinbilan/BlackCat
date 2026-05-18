#pragma once
#include <string>

namespace LabSys::Modules::SignalGen {

// Pure virtual interface for a Signal Generator device.
class SgBaseIf
{
public:
    virtual ~SgBaseIf() = default;

    virtual const std::string& instanceName() const = 0;

    // Channel lifecycle
    virtual void createChannels()      = 0;
    virtual void attachInputChannels() = 0;

    // Runtime control
    virtual void start()  = 0;
    virtual void stop()   = 0;
    virtual void tick()   = 0;  // called each scheduling step
};

} // namespace LabSys::Modules::SignalGen
