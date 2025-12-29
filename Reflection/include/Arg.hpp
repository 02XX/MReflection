#pragma once
#include <any>
#include <exception>
#include <functional>
#include <iostream>
namespace MReflection
{
class Arg
{
  private:
    std::any mValue;
    bool mIsLValueRef{false};
    bool mIsConst{false};

  public:
    template <typename T> Arg(T &&value) // value最后只能是左值引用或者是右值引用
    {
        mIsLValueRef = std::is_lvalue_reference_v<T>;
        mIsConst = std::is_const_v<std::remove_reference_t<T>>;
        // 构造函数不允许显示指定模板参数
        // 因此在构造函数万能引用中，T和Value只有两种结果：
        // T左值引用 value为左值(const or non-const)引用
        // T值类型 value为右值(const or non-const)引用
        if constexpr (std::is_lvalue_reference_v<T>) // T是左值引用
        {
            // value是左值引用
            // any会擦除传入参数的引用类型，如果不用std::ref包装，则会进行一次拷贝
            mValue = std::ref(value); // std::reference_wrapper<R> or std::reference_wrapper<const R>
        }
        else // T是值类型
        {
            // value是右值引用
            // any会擦除传入参数的引用类型，因此使用std::move进行一次移动语义。
            mValue = std::move(value); // R or const R
        }
    }
    template <typename TTarget> TTarget Cast()
    {
        using CleanTarget = std::remove_cv_t<std::remove_reference_t<TTarget>>;

        constexpr bool TargetIsRef = std::is_reference_v<TTarget>;
        constexpr bool TargetIsLValueRef = std::is_lvalue_reference_v<TTarget>;
        constexpr bool TargetIsRValueRef = std::is_rvalue_reference_v<TTarget>;
        constexpr bool TargetIsConst = std::is_const_v<std::remove_reference_t<TTarget>>;
        // T四种状态 (R&, const R&, R&&, const R&&)
        // Target六种状态 (R, const R, R&, const R&, R&&, const R&&)
        // 4 x 6 = 24种转换可能性
        if (mIsLValueRef) // mValue是左值引用
        {
            if constexpr (TargetIsRValueRef)
            {
                throw std::runtime_error("Cannot cast lvalue reference to rvalue reference");
            }
            else
            {
                if (mIsConst)
                {
                    // 存储的是 std::reference_wrapper<const CleanTarget>
                    if constexpr (!TargetIsConst && TargetIsLValueRef)
                    {
                        throw std::runtime_error("Cannot cast const reference to non-const reference");
                    }
                    else
                    {
                        auto ref = std::any_cast<std::reference_wrapper<const CleanTarget>>(mValue);
                        return ref.get();
                    }
                }
                else
                {
                    // 存储的是 std::reference_wrapper<CleanTarget>
                    auto ref = std::any_cast<std::reference_wrapper<CleanTarget>>(mValue);
                    return ref.get();
                }
            }
        }
        else // mValue是右值引用
        {
            if constexpr (TargetIsLValueRef)
            {
                throw std::runtime_error("Cannot cast rvalue to lvalue reference");
            }
            else
            {
                if (mIsConst)
                {
                    // 存储的是 const CleanTarget
                    auto &val = std::any_cast<const CleanTarget &>(mValue);
                    if constexpr (!TargetIsConst && TargetIsRValueRef)
                        throw std::runtime_error("Cannot cast const value to non-const reference");
                    else if constexpr (TargetIsRValueRef)
                        return std::move(val);
                    else
                        return val;
                }
                else
                {
                    // 存储的是 CleanTarget
                    auto &val = std::any_cast<CleanTarget &>(mValue);
                    if constexpr (TargetIsRValueRef)
                        return std::move(val);
                    else
                        return val;
                }
            }
        }
    }
};
} // namespace MReflection