#pragma once
#include "MemberInfo.hpp"
#include "MemberTypes.hpp"
#include <any>
#include <functional>
#include <iostream>
#include <tuple>
#include <type_traits>

namespace MReflection
{
class Arg
{
  private:
    std::any mValue;
    bool mIsRef{false};
    bool mIsConst{false};

  public:
    template <typename T> Arg(T &&value)
    {
        mIsRef = std::is_lvalue_reference_v<T>;
        mIsConst = std::is_const_v<std::remove_reference_t<T>>;
        if constexpr (std::is_lvalue_reference_v<T>)
        {
            mValue = std::ref(value); // 存储引用
        }
        else
        {
            mValue = std::forward<T>(value); // move or copy
        }
    }
    template <typename TTarget> TTarget Cast()
    {
        using CleanT = std::remove_cv_t<std::remove_reference_t<TTarget>>;
        if constexpr (std::is_lvalue_reference_v<TTarget>) // 目标类型是引用类型
        {

            if (!mIsRef) // 原始类型是值类型
            {
                if constexpr (!std::is_const_v<std::remove_reference_t<TTarget>>)
                    return std::any_cast<CleanT &>(mValue);
                else
                    return std::any_cast<const CleanT &>(mValue);
            }
            else //  原始类型是引用类型
            {
                if (mIsConst) // 原始类型是const引用
                {
                    // 原始类型是std::reference_wrapper<const T>
                    auto refWrapper = std::any_cast<std::reference_wrapper<const CleanT>>(mValue);
                    if constexpr (!std::is_const_v<std::remove_reference_t<TTarget>>)
                        throw std::runtime_error("不能将const引用转换为非const引用");
                    else
                        return refWrapper.get();
                }
                else // 原始类型是非const引用
                {
                    // 原始类型是std::reference_wrapper<T>
                    auto refWrapper = std::any_cast<std::reference_wrapper<CleanT>>(mValue);
                    return refWrapper.get();
                }
            }
        }
        else // 目标类型是值类型
        {
            if (mIsRef) // 原始类型是引用类型
            {
                if (mIsConst) // 原始类型是const引用
                {
                    // 原始类型是std::reference_wrapper<const T>
                    return std::any_cast<std::reference_wrapper<const CleanT>>(mValue).get();
                }
                else // 原始类型是非const引用
                {
                    // 原始类型是std::reference_wrapper<T>
                    return std::any_cast<std::reference_wrapper<CleanT>>(mValue).get();
                }
            }
            else // 原始类型是值类型
            {
                return std::any_cast<const CleanT &>(
                    mValue); // 返回引用给TTarget发生时再copy，避免any_cast的时候产生多余的copy
            }
        }
    }
};
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