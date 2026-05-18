#pragma once
#include "SgBaseIf.h"

namespace LabSys::Modules::SignalGen {

// Concrete base for all Signal Generator devices.
// Vendor-specific variants (SgVendorA, ...) extend this class.
class SgBase : public SgBaseIf
{
public:
    explicit SgBase(std::string name);
    ~SgBase() override = default;

    const std::string& instanceName() const override;

    void createChannels()      override;
    void attachInputChannels() override;
    void start()               override;
    void stop()                override;
    void tick()                override;

private:
    std::string m_name;
    // TODO: output channel handles
    // TODO: input channel handles
};

} // namespace LabSys::Modules::SignalGen
