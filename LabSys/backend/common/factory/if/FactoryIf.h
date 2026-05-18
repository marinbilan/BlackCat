#pragma once
#include <string>
#include <memory>

namespace LabSys::Factory {

// Base class for all creatable objects
struct ObjectBase
{
    virtual ~ObjectBase() = default;
    virtual const std::string& typeName() const = 0;
};

// FactoryIf — pure virtual factory interface.
// Concrete implementations register constructors via REGISTER_CLASS macro.
class FactoryIf
{
public:
    virtual ~FactoryIf() = default;

    // Create an object by its registered type name.
    // Returns nullptr if type is not registered.
    virtual std::shared_ptr<ObjectBase> create(const std::string& typeName) const = 0;

    virtual bool isRegistered(const std::string& typeName) const = 0;
};

} // namespace LabSys::Factory
