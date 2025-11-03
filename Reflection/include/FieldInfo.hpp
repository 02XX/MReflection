#pragma once
#include "MemberInfo.hpp"
#include "MemberTypes.hpp"
#include <any>
#include <functional>

namespace MReflection
{
class FieldInfo final : public MemberInfo
{
  public:
    template <class Class, typename TField>
    FieldInfo(const std::string &name, TField Class::*fieldPointer) : MemberInfo(name)
    {
        mGetter = [fieldPointer](std::any obj) -> std::any {
            auto &instance = std::any_cast<std::reference_wrapper<const Class>>(obj).get();
            return instance.*fieldPointer;
        };
        mSetter = [fieldPointer](std::any obj, std::any value) {
            auto &instance = std::any_cast<std::reference_wrapper<Class>>(obj).get();
            instance.*fieldPointer =
                std::any_cast<typename std::remove_reference<decltype(instance.*fieldPointer)>::type>(value);
        };
    }

  public:
    template <class TRet, class Class> TRet GetValue(const Class &obj) const
    {
        return std::any_cast<TRet>(mGetter(std::cref(obj)));
    }
    template <class TValue, class Class> void SetValue(Class &obj, TValue &&value) const
    {
        mSetter(std::ref(obj), std::forward<TValue>(value));
    }

  private:
    MemberType mMemberType{MemberType::Field};
    std::function<std::any(std::any)> mGetter{nullptr};
    std::function<void(std::any, std::any)> mSetter{nullptr};
};
} // namespace MReflection