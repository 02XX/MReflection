#include "MReflection.hpp"
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

TEST(ReflectionTest, GetSetField)
{
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
    auto animal = std::make_shared<Animal>("GenericAnimal", 5);
    MReflection::AddClass("Animal")
        .AddField("Name", &Animal::Name)
        .AddField("Age", &Animal::Age)
        .AddMethod("Speak", &Animal::Speak)
        .AddMethod("CelebrateBirthday", &Animal::CelebrateBirthday);
    MReflection::Registry &registry = MReflection::Registry::GetInstance();
    auto nameField = registry.GetType("Animal")->GetField("Name");
    EXPECT_EQ(nameField->GetValue<std::string>(*animal), "GenericAnimal");
    nameField->SetValue<std::string>(*animal, "NewAnimalName");
    EXPECT_EQ(nameField->GetValue<std::string>(*animal), "NewAnimalName");
    auto celebrateMethod = registry.GetType("Animal")->GetMethod("CelebrateBirthday");
    celebrateMethod->Invoke(*animal);
}
