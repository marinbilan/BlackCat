#pragma once

namespace LabSys::Modules::Pid {

class PidMasterIf
{
public:
    virtual ~PidMasterIf() = default;

    virtual void onCreate()                    = 0;
    virtual void onInit()                      = 0;
    virtual void onCreateChannels()            = 0;
    virtual void onAttach()                    = 0;
    virtual void run()                         = 0;
};

} // namespace LabSys::Modules::Pid
