#include "gtest/gtest.h"
#include <gtest/gtest.h>
#include <type_traits>

template <typename T> void PrintTType(T &param)
{
    if (std::is_lvalue_reference_v<T>)
    {
        GTEST_LOG_(INFO) << "T is LValue Reference";
    }
    else if (std::is_rvalue_reference_v<T>)
    {
        GTEST_LOG_(INFO) << "T is RValue Reference";
    }
    else
    {
        GTEST_LOG_(INFO) << "T is Value Type";
    }
}

void identify(const char (&)[6])
{
    std::cout << "1. 函数重载：探测到 [左值 (Lvalue)]" << std::endl;
}

// 如果传入的是右值，编译器会优先匹配这个版本
void identify(const char (&&)[6])
{
    std::cout << "1. 函数重载：探测到 [右值 (Rvalue)]" << std::endl;
}

TEST(UniversalReferenceTest, ValueType)
{
    int a = 1;
    PrintTType(a);
}