#pragma once
#include "SgBase.h"

namespace LabSys::Modules::SignalGen {

// Vendor A signal generator — extends SgBase with hardware-specific behaviour.
class SgVendorA : public SgBase
{
public:
    explicit SgVendorA(std::string name);
    ~SgVendorA() override = default;

    void tick() override;  // override to drive vendor SDK / hardware

    // TODO: vendor-specific methods
};

} // namespace LabSys::Modules::SignalGen
