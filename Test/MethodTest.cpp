
#include "Int.hpp"
#include "MethodInfo.hpp"
#include <gtest/gtest.h>
#include <memory>
#include <string>

using namespace MReflection;

class Animal
{
  public:
    std::string Name;
    Int Age;
    Animal(const std::string &name, int age) : Name(name), Age(age)
    {
    }
    virtual ~Animal() = default;
    const std::string &GetName() const
    {
        return Name;
    }
    void SetName(const std::string &newName)
    {
        Name = newName;
    }
    const Int &GetAge() const
    {
        return Age;
    }
    void SetAge(const Int &newAge)
    {
        Age = newAge;
    }
    virtual std::string Speak() const
    {
        return "Animal sound";
    }
};

// TEST(MethodTest, InvokeMethod_1)
// {
//     auto animal = std::make_shared<Animal>("GenericAnimal", 5);
//     MethodInfo getName("GetName", &Animal::GetName, nullptr);
//     MethodInfo setName("SetName", &Animal::SetName, nullptr);
//     std::string currentName = std::any_cast<const std::string &>(getName.Invoke(*animal));
//     EXPECT_EQ(currentName, "GenericAnimal");
//     std::string newName = "NewAnimalName";
//     setName.Invoke(*animal, newName);
//     currentName = std::any_cast<const std::string &>(getName.Invoke(*animal));
//     EXPECT_EQ(currentName, "NewAnimalName");
// }
TEST(MethodTest, InvokeMethod_2)
{
    auto animal = std::make_shared<Animal>("GenericAnimal", 5);
    MethodInfo getAge("GetAge", &Animal::GetAge, nullptr);
    MethodInfo setAge("SetAge", &Animal::SetAge, nullptr);
    // Int currentAge = std::any_cast<const Int &>(getAge.Invoke(*animal));
    // EXPECT_EQ(currentAge.mValue, 5);
    Int newAge(10);
    setAge.Invoke(*animal, newAge);
    // currentAge = std::any_cast<const Int &>(getAge.Invoke(*animal));
    // EXPECT_EQ(currentAge.mValue, 10);
}