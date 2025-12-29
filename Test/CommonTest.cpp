#include "gtest/gtest.h"
#include <gtest/gtest.h>
#include <tuple>
#include <type_traits>

class Animal
{
  public:
    Animal(int age) : mAge(age)
    {
    }
    Animal(const Animal &other) : mAge(other.mAge)
    {
        GTEST_LOG_(INFO) << "Animal Copy Constructor";
    }
    Animal(Animal &&other) noexcept : mAge(other.mAge)
    {
        GTEST_LOG_(INFO) << "Animal Move Constructor";
    }
    int mAge;
    int GetAge() const
    {
        return mAge;
    }
};
class Int
{
  public:
    int mValue;
    Int(int value) : mValue(value)
    {
    }
    Int(const Int &other) : mValue(other.mValue)
    {
        GTEST_LOG_(INFO) << "Int Copy Constructor";
    }
    Int(Int &&other) noexcept : mValue(other.mValue)
    {
        GTEST_LOG_(INFO) << "Int Move Constructor";
    }
    Int &operator=(const Int &other)
    {
        GTEST_LOG_(INFO) << "Int Copy Assignment";
        mValue = other.mValue;
        return *this;
    }
    Int &operator=(Int &&other) noexcept
    {
        GTEST_LOG_(INFO) << "Int Move Assignment";
        mValue = other.mValue;
        return *this;
    }
};
class Float
{
  public:
    float mValue;
    Float(float value) : mValue(value)
    {
    }
    Float(const Float &other) : mValue(other.mValue)
    {
        GTEST_LOG_(INFO) << "Float Copy Constructor";
    }
    Float(Float &&other) noexcept : mValue(other.mValue)
    {
        GTEST_LOG_(INFO) << "Float Move Constructor";
    }
    Float &operator=(const Float &other)
    {
        GTEST_LOG_(INFO) << "Float Copy Assignment";
        mValue = other.mValue;
        return *this;
    }
};
template <class... TArgs> void AcceptPerfect(TArgs &&...args)
{
    using TupleType = std::tuple<TArgs &&...>; // std::tuple<Int&, Float&&>
    auto f = [](auto &&...xs) {
        (..., (GTEST_LOG_(INFO) << "LValue: " << std::is_lvalue_reference_v<decltype(xs)> << ", RValue: "
                                << std::is_rvalue_reference_v<decltype(xs)>));
    };
    auto f2 = [&](std::any obj) {
        auto &p = std::any_cast<std::reference_wrapper<TupleType>>(obj).get();
        std::apply(f, std::move(p));
    };
    TupleType params = std::forward_as_tuple(std::forward<TArgs>(args)...);
    f2(std::ref(params));
}
// TEST(CommonTest, AnyTest)
// {
//     Int intObj(10);
//     Float floatObj(30.5f);
//     std::any a1 = intObj;       // Copy
//     std::any a2 = Float(20.5f); // Move

//     Int a1Value = std::any_cast<Int>(a1);
//     Float a2Value = std::any_cast<Float>(a2);
//     // 随意move应为 std::any内存储的是值类型与外部对象无关
// }
TEST(CommonTest, Forward_as_tuple)
{
    Int intObj(10);
    AcceptPerfect(intObj, Float(30.5f));
}