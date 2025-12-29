#include "MethodInfo.hpp"
#include <gtest/gtest.h>
#include <memory>
#include <string>

using namespace MReflection;
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
class Animal
{
  public:
    Animal(const std::string &name, int age) : Name(name), Age(age)
    {
    }
    virtual ~Animal() = default;
    std::string Name;
    int Age;
    virtual std::string Speak() const
    {
        return "Animal sound";
    }
    void CelebrateBirthday()
    {
        GTEST_LOG_(INFO) << "Happy Birthday, " << Name << "!";
        Age += 1;
    }
    std::string SetName(const std::string &newName)
    {
        Name = newName;
        return Name;
    }
    void ChangeName(std::string &nameRef)
    {
        nameRef = "ChangedName";
    }
    void Test(Int intObj, Float floatObj)
    {
        GTEST_LOG_(INFO) << "Int Value: " << intObj.mValue << ", Float Value: " << floatObj.mValue;
    }
};

class Dog : public Animal
{
  public:
    Dog(const std::string &name, int age, const std::string &breed) : Animal(name, age), Breed(breed)
    {
    }
    std::string Breed;
    std::string Speak() const override
    {
        return "Woof!";
    }
};
TEST(ArgTest, Cast)
{
    const Int intObj(10);
    // 目标是引用
    //  原始是引用
    //      原始是const
            {
                
            }
    //  原始是值类型
    //      原始是non-const
    // 目标是值类型
    //  原始是引用
    //      原始是const
    //     原始是non-const
    //  原始是值类型
    //      原始是const
    //      原始是non-const
}

// TEST(MethodInfoTest, LRValue)
// {
//     auto animal = std::make_unique<Animal>("123", 123);
//     MethodInfo m("Test", &Animal::Test, nullptr);
//     Int intObj(10);
//     Float floatObj(20.5f);
//     m.Invoke(*animal, intObj, Float(20.5f));
// }
