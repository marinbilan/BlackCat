#pragma once

namespace LabSys::Modules::SignalGen {

// Pure virtual interface for the Signal Generator Master device.
// The Master is always present; it creates and manages SG device instances
// based on the JSON configuration received from the Runtime Manager.
class SignalGenMasterIf
{
public:
    virtual ~SignalGenMasterIf() = default;

    virtual void onCreate()                    = 0;
    virtual void onInit()                      = 0;
    virtual void onCreateChannels()            = 0;
    virtual void onAttach()                    = 0;
    virtual void run()                         = 0;
};

} // namespace LabSys::Modules::SignalGen
