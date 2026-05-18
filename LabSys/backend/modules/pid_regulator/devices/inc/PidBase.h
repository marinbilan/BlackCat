#pragma once
#include "PidBaseIf.h"

namespace LabSys::Modules::Pid {

// Discrete PID regulator device (Z-transform implementation target).
class PidBase : public PidBaseIf
{
public:
    explicit PidBase(std::string name);
    ~PidBase() override = default;

    const std::string& instanceName() const override;

    void createChannels()      override;
    void attachInputChannels() override;
    void start()               override;
    void stop()                override;
    void tick()                override;

private:
    std::string m_name;

    // PID gains
    double m_kp{1.0};
    double m_ki{0.0};
    double m_kd{0.0};

    // State
    double m_integral{0.0};
    double m_prevError{0.0};
};

} // namespace LabSys::Modules::Pid
