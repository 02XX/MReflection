#include "TypeInfo.hpp"
#include <stdexcept>

namespace MReflection
{
FieldInfo *TypeInfo::GetField(const std::string &name) const
{
    if (mFields.contains(name))
    {
        return mFields.at(name).get();
    }
    throw std::runtime_error("Field not found: " + name);
}
MethodInfo *TypeInfo::GetMethod(const std::string &name) const
{
    if (mMethods.contains(name))
    {
        return mMethods.at(name).get();
    }
    throw std::runtime_error("Method not found: " + name);
}
std::vector<MethodInfo> TypeInfo::GetMethods() const
{
    std::vector<MethodInfo> methods;
    for (const auto &[name, method] : mMethods)
    {
        methods.push_back(*method);
    }
    return methods;
}
std::vector<FieldInfo> TypeInfo::GetFields() const
{
    std::vector<FieldInfo> fields;
    for (const auto &[name, field] : mFields)
    {
        fields.push_back(*field);
    }
    return fields;
}
} // namespace MReflection