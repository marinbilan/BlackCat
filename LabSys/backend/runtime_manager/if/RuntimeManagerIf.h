#pragma once
// RuntimeManagerIf.h — pure virtual interface for the Runtime Manager.
// All state transitions are declared here; concrete class implements them.

namespace LabSys {

class RuntimeManagerIf
{
public:
    virtual ~RuntimeManagerIf() = default;

    // State machine transitions
    virtual void onCreate()        = 0;  // Instruct masters to create devices
    virtual void onInit()          = 0;  // Send config params to devices
    virtual void onCreateChannels()= 0;  // Each device registers output channels
    virtual void onAttach()        = 0;  // Devices attach to input channels

    // Main loop
    virtual void run()             = 0;
};

} // namespace LabSys
