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
    if (mBaseType)
    {
        return mBaseType->GetField(name);
    }
    throw std::runtime_error("Field not found: " + name);
}
MethodInfo *TypeInfo::GetMethod(const std::string &name) const
{
    if (mMethods.contains(name))
    {
        return mMethods.at(name).get();
    }
    if (mBaseType)
    {
        return mBaseType->GetMethod(name);
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
    if (mBaseType)
    {
        auto baseMethods = mBaseType->GetMethods();
        for (const auto &method : baseMethods)
        {
            if (mMethods.contains(method.GetName())) // Skip overridden methods
                continue;
            methods.push_back(method);
        }
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
    if (mBaseType)
    {
        auto baseFields = mBaseType->GetFields();
        for (const auto &field : baseFields)
        {
            fields.push_back(field);
        }
    }
    return fields;
}
} // namespace MReflection