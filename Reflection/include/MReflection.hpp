#pragma once
#include "TypeInfo.hpp"
#include "TypeInfoBuilder.hpp"
#include <memory>
#include <string>
#include <unordered_map>

namespace MReflection
{
class Registry final
{
    friend class TypeInfoBuilder;
    friend TypeInfoBuilder AddClass(std::string typeName);

  public:
    static Registry &GetInstance();
    TypeInfo *GetType(const std::string &name) const;

  private:
    std::unordered_map<std::string, std::unique_ptr<TypeInfo>> mTypes;
};
TypeInfoBuilder AddClass(std::string typeName);
} // namespace MReflection