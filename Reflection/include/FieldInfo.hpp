#pragma once
#include "MemberInfo.hpp"
#include "MemberTypes.hpp"
#include <any>
#include <functional>

namespace MReflection
{
class TypeInfo;
class FieldInfo final : public MemberInfo
{
  public:
    template <class TTClass, typename TField>
    FieldInfo(const std::string &name, TField TTClass::*fieldPointer, TypeInfo *declaringType)
        : MemberInfo(name, declaringType)
    {
        mGetter = [fieldPointer](std::any obj) -> std::any {
            auto &instance = std::any_cast<std::reference_wrapper<const TTClass>>(obj).get();
            return instance.*fieldPointer;
        };
        mSetter = [fieldPointer](std::any obj, std::any value) {
            auto &instance = std::any_cast<std::reference_wrapper<TTClass>>(obj).get();
            instance.*fieldPointer =
                std::any_cast<typename std::remove_reference<decltype(instance.*fieldPointer)>::type>(value);
        };
    }
    inline MemberTypes GetMemberType() const override
    {
        return MemberTypes::Field;
    }

  public:
    template <class TRet, class TTClass> TRet GetValue(const TTClass &obj) const
    {
        return std::any_cast<TRet>(mGetter(std::cref(obj)));
    }
    template <class TValue, class TTClass> void SetValue(TTClass &obj, TValue &&value) const
    {
        mSetter(std::ref(obj), std::forward<TValue>(value));
    }

  private:
    std::function<std::any(std::any)> mGetter{nullptr};
    std::function<void(std::any, std::any)> mSetter{nullptr};
};
} // namespace MReflection