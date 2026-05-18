#include "Factory.h"
#include <stdexcept>

namespace LabSys::Factory {

Factory& Factory::instance()
{
    static Factory s_instance;
    return s_instance;
}

void Factory::registerType(const std::string& typeName, CreatorFn fn)
{
    m_registry[typeName] = std::move(fn);
}

std::shared_ptr<ObjectBase> Factory::create(const std::string& typeName) const
{
    auto it = m_registry.find(typeName);
    if (it == m_registry.end())
        return nullptr;
    return it->second();
}

bool Factory::isRegistered(const std::string& typeName) const
{
    return m_registry.count(typeName) > 0;
}

} // namespace LabSys::Factory
