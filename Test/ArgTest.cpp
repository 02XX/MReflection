#include "Arg.hpp"
#include "Int.hpp"
#include <boost/type_index.hpp>
#include <gtest/gtest.h>
#include <utility>
using namespace MReflection;
TEST(ArgTest, Cast)
{
    Int intObj(10);
    Int &intLRef = intObj;
    Int &&intRRef = Int(20);
    const Int constIntObj(15);
    const Int &constIntLRef = constIntObj;
    const Int &&constIntRRef = Int(25);

    // 原始是左值引用
    Arg arg1(intLRef);
    Int a1 = arg1.Cast<Int>(); // Copy
    Int &a2 = arg1.Cast<Int &>();
    EXPECT_THROW(arg1.Cast<Int &&>(), std::runtime_error);
    Int a3 = arg1.Cast<const Int>(); // Copy
    const Int &a4 = arg1.Cast<const Int &>();
    EXPECT_THROW(arg1.Cast<const Int &&>(), std::runtime_error);
    // 原始是const左值引用
    Arg arg2(constIntLRef);
    Int a5 = arg2.Cast<Int>(); // Copy
    EXPECT_THROW(arg2.Cast<Int &>(), std::runtime_error);
    EXPECT_THROW(arg2.Cast<Int &&>(), std::runtime_error);
    const Int a6 = arg2.Cast<const Int>(); // Copy
    const Int &a7 = arg2.Cast<const Int &>();
    EXPECT_THROW(arg2.Cast<const Int &&>(), std::runtime_error);
    // 原始是右值引用
    Arg arg3(std::move(intRRef));
    Int a8 = arg3.Cast<Int>(); // Move
    EXPECT_THROW(arg3.Cast<Int &>(), std::runtime_error);
    Int a9 = arg3.Cast<Int &&>();           // Move
    const Int a10 = arg3.Cast<const Int>(); // Move
    EXPECT_THROW(arg3.Cast<const Int &>(), std::runtime_error);
    const Int &&a11 = arg3.Cast<const Int &&>(); // Move
    // 原始是const右值引用
    Arg arg4(std::move(constIntRRef));
    Int a12 = arg4.Cast<Int>(); // Move
    EXPECT_THROW(arg4.Cast<Int &>(), std::runtime_error);
    EXPECT_THROW(arg4.Cast<Int &&>(), std::runtime_error);
    const Int a13 = arg4.Cast<const Int>(); // Move
    EXPECT_THROW(arg4.Cast<const Int &>(), std::runtime_error);
    const Int a14 = arg4.Cast<const Int &&>(); // Move
}
