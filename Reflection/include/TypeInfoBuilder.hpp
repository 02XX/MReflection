#pragma once
#include "TypeInfo.hpp"
#include <memory>

namespace MReflection
{
class TypeInfoBuilder final
{
  public:
    TypeInfoBuilder(std::string typeName) : mTypeInfo(std::make_unique<TypeInfo>(typeName))
    {
    }
    ~TypeInfoBuilder();
    template <class Class, typename TField>
    TypeInfoBuilder &AddField(const std::string &fieldName, TField Class::*fieldPointer)
    {
        mTypeInfo->mFields[fieldName] = std::make_unique<FieldInfo>(fieldName, fieldPointer);
        return *this;
    }
    template <class Class, typename TRet, typename... TArgs>
    TypeInfoBuilder &AddMethod(const std::string &methodName, TRet (Class::*methodPointer)(TArgs...))
    {
        mTypeInfo->mMethods[methodName] = std::make_unique<MethodInfo>(methodName, methodPointer);
        return *this;
    }
    template <class Class, typename TRet, typename... TArgs>
    TypeInfoBuilder &AddMethod(const std::string &methodName, TRet (Class::*methodPointer)(TArgs...) const)
    {
        mTypeInfo->mMethods[methodName] = std::make_unique<MethodInfo>(methodName, methodPointer);
        return *this;
    }

  private:
    std::unique_ptr<TypeInfo> mTypeInfo{nullptr};
};
} // namespace MReflection