#pragma once
#include "FieldInfo.hpp"
#include "MethodInfo.hpp"
#include <string>
#include <vector>

namespace MReflection
{
class IReflect
{
  public:
    virtual ~IReflect() = default;
    virtual MethodInfo *GetMethod(const std::string &name) const = 0;
    virtual FieldInfo *GetField(const std::string &name) const = 0;
    virtual std::vector<MethodInfo> GetMethods() const = 0;
    virtual std::vector<FieldInfo> GetFields() const = 0;
};
} // namespace MReflection