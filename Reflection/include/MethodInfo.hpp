#pragma once
#include "MemberInfo.hpp"
#include "MemberTypes.hpp"
#include <any>
#include <functional>
#include <tuple>

namespace MReflection
{
using InvokeHandler = std::function<std::any(std::any, std::any)>;
class MethodInfo final : public MemberInfo
{
  public:
    template <class TClass, typename TRet, typename... TArgs>
    MethodInfo(const std::string &name, TRet (TClass::*methodPointer)(TArgs...), TypeInfo *declaringType)
        : MemberInfo(name, declaringType)
    {
        mInvokeHandler = [methodPointer](std::any obj, std::any args) -> std::any {
            auto &instance = std::any_cast<std::reference_wrapper<TClass>>(obj).get();
            auto &tupleArgs = std::any_cast<std::tuple<TArgs &&...> &>(args);
            return std::apply(
                [&instance, methodPointer](TArgs &&...unpackedArgs) {
                    if constexpr (std::is_void<TRet>::value)
                    {
                        (instance.*methodPointer)(std::forward<TArgs>(unpackedArgs)...);
                        return std::any{};
                    }
                    else
                    {
                        return (instance.*methodPointer)(std::forward<TArgs>(unpackedArgs)...);
                    }
                },
                tupleArgs);
        };
        mIsConst = false;
    }
    template <class TClass, typename TRet, typename... TArgs>
    MethodInfo(const std::string &name, TRet (TClass::*methodPointer)(TArgs...) const, TypeInfo *declaringType)
        : MemberInfo(name, declaringType)
    {
        mInvokeHandler = [methodPointer](std::any obj, std::any args) -> std::any {
            auto &instance = std::any_cast<std::reference_wrapper<const TClass>>(obj).get();
            auto &tupleArgs = std::any_cast<std::tuple<TArgs &&...> &>(args);
            return std::apply(
                [&instance, methodPointer](TArgs &&...unpackedArgs) {
                    if constexpr (std::is_void<TRet>::value)
                    {
                        (instance.*methodPointer)(std::forward<TArgs>(unpackedArgs)...);
                        return std::any{};
                    }
                    else
                    {
                        return (instance.*methodPointer)(std::forward<TArgs>(unpackedArgs)...);
                    }
                },
                tupleArgs);
        };
        mIsConst = true;
    }
    inline MemberTypes GetMemberType() const override
    {
        return MemberTypes::Method;
    }

  public:
    template <class TClass, typename... TArgs> std::any Invoke(TClass &obj, TArgs &&...args) const
    {
        if (mIsConst)
        {
            return mInvokeHandler(std::cref(obj), std::forward_as_tuple(std::forward<TArgs>(args)...));
        }
        return mInvokeHandler(std::ref(obj), std::forward_as_tuple(std::forward<TArgs>(args)...));
    }

  private:
    bool mIsConst{false};
    bool mIsStatic{false};
    bool mIsVirtual{false};
    bool mIsPublic{true};
    bool mIsProtected{false};
    bool mIsPrivate{false};
    InvokeHandler mInvokeHandler{nullptr};
};
} // namespace MReflection