#pragma once
#include "FieldInfo.hpp"
#include "IReflect.hpp"
#include "MemberInfo.hpp"
#include "MemberTypes.hpp"
#include "MethodInfo.hpp"
#include <memory>
#include <string>
#include <unordered_map>

namespace MReflection
{
class TypeInfo final : public MemberInfo, public IReflect
{
    friend class TypeInfoBuilder;

  public:
    TypeInfo(const std::string &name) : MemberInfo(name)
    {
    }
    FieldInfo *GetField(const std::string &name) const override;
    MethodInfo *GetMethod(const std::string &name) const override;
    std::vector<MethodInfo> GetMethods() const override;
    std::vector<FieldInfo> GetFields() const override;
    TypeInfo *GetBaseType() const;

  private:
    MemberType mMemberType{MemberType::TypeInfo};
    std::unordered_map<std::string, std::unique_ptr<FieldInfo>> mFields;
    std::unordered_map<std::string, std::unique_ptr<MethodInfo>> mMethods;
    TypeInfo *mBaseType{nullptr};
};
} // namespace MReflection