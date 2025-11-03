#pragma once
#include "TypeInfoBuilder.hpp"

namespace MReflection
{
template <class TClass> class TypeBuilder : public TypeInfoBuilder
{
  public:
    TypeBuilder() : TypeInfoBuilder(typeid(TClass).name())
    {
    }
    ~TypeBuilder() override = default;
    template <class TBase> TypeBuilder &AddBaseClass()
    {
        TypeInfoBuilder::AddBaseClass(typeid(TBase).name());
        return *this;
    }
    virtual TypeInfoBuilder &AddBaseClass(const std::string &baseTClassName) override
    {
        return TypeInfoBuilder::AddBaseClass(baseTClassName);
    }
    virtual TypeInfoBuilder &AddBaseClass(TypeInfo *baseTypeInfo) override
    {
        return TypeInfoBuilder::AddBaseClass(baseTypeInfo);
    }
};
} // namespace MReflection