#include "SgVendorA.h"

namespace LabSys::Modules::SignalGen {

SgVendorA::SgVendorA(std::string name) : SgBase(std::move(name)) {}

void SgVendorA::tick()
{
    // TODO: read sample from vendor hardware SDK, push to output channel
    SgBase::tick(); // call base for now
}

} // namespace LabSys::Modules::SignalGen
