#pragma once
#include <string>

namespace LabSys::Modules::Pid {

class PidBaseIf
{
public:
    virtual ~PidBaseIf() = default;

    virtual const std::string& instanceName() const = 0;

    virtual void createChannels()      = 0;
    virtual void attachInputChannels() = 0;
    virtual void start()  = 0;
    virtual void stop()   = 0;
    virtual void tick()   = 0;
};

} // namespace LabSys::Modules::Pid
