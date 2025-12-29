#pragma once
#include "Arg.hpp"
#include "MemberInfo.hpp"
#include "MemberTypes.hpp"
#include <any>
#include <functional>
#include <iostream>
#include <tuple>
#include <type_traits>


namespace MReflection
{

class MethodInfo final : public MemberInfo
{
    using InvokeHandler = std::function<std::any(std::any, std::any)>;

  public:
    template <class TClass, typename TRet, typename... TArgs>
    MethodInfo(const std::string &name, TRet (TClass::*methodPointer)(TArgs...), TypeInfo *declaringType)
        : MemberInfo(name, declaringType)
    {
        auto f = [](auto &&...xs) {
            (..., (std::cout << "LValue: " << std::is_lvalue_reference_v<decltype(xs)> << ", RValue: "
                             << std::is_rvalue_reference_v<decltype(xs)>));
        };
        mInvokeHandler = [methodPointer, f](std::any obj, std::any args) -> std::any {
            auto &instance = std::any_cast<std::reference_wrapper<TClass>>(obj).get();
            auto &tupleArgs = std::any_cast<std::reference_wrapper<std::tuple<TArgs...>>>(args).get();
            return {};
        };
        mIsConst = false;
    }
    template <class TClass, typename TRet, typename... TArgs>
    MethodInfo(const std::string &name, TRet (TClass::*methodPointer)(TArgs...) const, TypeInfo *declaringType)
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
        mIsConst = true;
    }
    inline MemberTypes GetMemberType() const override
    {
        return MemberTypes::Method;
    }

  public:
    template <class TClass, typename... TArgs> std::any Invoke(TClass &obj, TArgs &&...args) const
    {
        auto tupleArgs = std::forward_as_tuple(std::forward<TArgs>(args)...);
        auto t = typeid(tupleArgs).name();
        if (mIsConst)
        {
            return mInvokeHandler(std::cref(obj), std::ref(tupleArgs));
        }
        return mInvokeHandler(std::ref(obj), std::ref(tupleArgs));
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