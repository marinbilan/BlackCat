#pragma once
#include "FactoryIf.h"
#include <functional>
#include <unordered_map>

namespace LabSys::Factory {

// Factory — singleton that maps type-name strings to constructor lambdas.
// Usage:
//   REGISTER_CLASS(MyDevice)
//   auto obj = Factory::instance().create("MyDevice");
class Factory : public FactoryIf
{
public:
    using CreatorFn = std::function<std::shared_ptr<ObjectBase>()>;

    static Factory& instance();

    // Called by REGISTER_CLASS macro at static-init time
    void registerType(const std::string& typeName, CreatorFn fn);

    std::shared_ptr<ObjectBase> create(const std::string& typeName) const override;
    bool isRegistered(const std::string& typeName)                  const override;

private:
    Factory() = default;
    std::unordered_map<std::string, CreatorFn> m_registry;
};

// ── REGISTER_CLASS macro ──────────────────────────────────────────────────────
// Place once per translation unit (typically in the .cpp of each device).
// Example:  REGISTER_CLASS(LabSys::Devices::SignalGen)
#define REGISTER_CLASS(ClassName)                                              \
    static const bool _registered_##ClassName = []() {                        \
        ::LabSys::Factory::Factory::instance().registerType(                   \
            #ClassName,                                                        \
            []() -> std::shared_ptr<::LabSys::Factory::ObjectBase> {          \
                return std::make_shared<ClassName>();                          \
            });                                                                \
        return true;                                                           \
    }()

} // namespace LabSys::Factory
