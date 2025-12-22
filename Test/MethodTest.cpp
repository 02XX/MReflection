#include "MethodInfo.hpp"
#include <gtest/gtest.h>
#include <string>

using namespace MReflection;

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

TEST(MethodInfoTest, InvokeMethod)
{
    auto animal = std::make_shared<Animal>("GenericAnimal", 5);
    MethodInfo speakMethod("Speak", &Animal::Speak, nullptr);
    MethodInfo celebrateMethod("CelebrateBirthday", &Animal::CelebrateBirthday, nullptr);
    MethodInfo setNameMethod("SetName", &Animal::SetName, nullptr);
    MethodInfo changeNameMethod("ChangeName", &Animal::ChangeName, nullptr);
    std::any result = speakMethod.Invoke(*animal);
    EXPECT_EQ(std::any_cast<std::string>(result), "Animal sound");

    celebrateMethod.Invoke(*animal);
    EXPECT_EQ(animal->Age, 6);
    const std::string newName("NewName");
    result = setNameMethod.Invoke(*animal, newName);
    EXPECT_EQ(std::any_cast<std::string>(result), "NewName");

    std::string nameRef = "OldName";
    changeNameMethod.Invoke(*animal, nameRef);
    EXPECT_EQ(nameRef, "ChangedName");
}
