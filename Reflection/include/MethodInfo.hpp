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
    template <class Class, typename TRet, typename... TArgs>
    MethodInfo(const std::string &name, TRet (Class::*methodPointer)(TArgs...)) : MemberInfo(name)
    {
        mInvokeHandler = [methodPointer](std::any obj, std::any args) -> std::any {
            auto &instance = std::any_cast<std::reference_wrapper<Class>>(obj).get();
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
    template <class Class, typename TRet, typename... TArgs>
    MethodInfo(const std::string &name, TRet (Class::*methodPointer)(TArgs...) const) : MemberInfo(name)
    {
        mInvokeHandler = [methodPointer](std::any obj, std::any args) -> std::any {
            auto &instance = std::any_cast<std::reference_wrapper<const Class>>(obj).get();
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

  public:
    template <class Class, typename... TArgs> std::any Invoke(Class &obj, TArgs &&...args) const
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
    MemberType mMemberType{MemberType::Method};
};
} // namespace MReflection