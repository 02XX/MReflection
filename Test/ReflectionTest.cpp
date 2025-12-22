#include "MReflection.hpp"
#include "gtest/gtest.h"
#include <gtest/gtest.h>
#include <iostream>
#include <string>

using namespace MReflection;

class Animal
{
  public:
    Animal(const std::string &name, int age) : Name(name), Age(age)
    {
        GTEST_LOG_(INFO) << "Animal constructor called for " << Name;
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
        std::cout << "Happy Birthday, " << Name << "!" << std::endl;
        Age += 1;
    }
    std::string SetName(const std::string &newName)
    {
        Name = newName;
        return Name;
    }
};

class Dog : public Animal
{
  public:
    Dog(const std::string &name, int age, const std::string &breed) : Animal(name, age), Breed(breed)
    {
        GTEST_LOG_(INFO) << "Dog constructor called for " << Name;
    }
    Dog(const std::string &name) : Animal(name, 0), Breed("Unknown")
    {
        GTEST_LOG_(INFO) << "Dog constructor called for " << Name;
    }
    std::string Breed;
    std::string Speak() const override
    {
        return "Woof!";
    }
    void Eat()
    {
        std::cout << Name << " is eating." << std::endl;
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
TEST(ReflectionTest, InheritedMethod)
{
    MReflection::Registry &registry = MReflection::Registry::GetInstance();
    auto dog = std::make_shared<Dog>("Buddy", 3, "Golden Retriever");
    MReflection::AddClass("Animal")
        .AddField("Name", &Animal::Name)
        .AddField("Age", &Animal::Age)
        .AddMethod("Speak", &Animal::Speak)
        .AddMethod("CelebrateBirthday", &Animal::CelebrateBirthday)
        .AddMethod("SetName", &Animal::SetName);
    MReflection::AddClass("Dog")
        .AddBaseClass("Animal")
        .AddField("Breed", &Dog::Breed)
        .AddMethod("Speak", &Dog::Speak)
        .AddMethod("Eat", &Dog::Eat);
    auto dogFields = registry.GetType("Dog")->GetFields();
    for (const auto &field : dogFields)
    {
        GTEST_LOG_(INFO) << "Dog Field: " << field->GetName();
    }
    auto dogMethods = registry.GetType("Dog")->GetMethods();
    for (const auto &method : dogMethods)
    {
        GTEST_LOG_(INFO) << "Dog Method: " << method->GetName();
    }
}

TEST(ReflectionTest, InheritedMethod_2)
{
    MReflection::Registry &registry = MReflection::Registry::GetInstance();
    auto dog = std::make_shared<Dog>("Buddy", 3, "Golden Retriever");
    MReflection::AddClass<Animal>()
        .AddField("Name", &Animal::Name)
        .AddField("Age", &Animal::Age)
        .AddMethod("Speak", &Animal::Speak)
        .AddMethod("CelebrateBirthday", &Animal::CelebrateBirthday)
        .AddMethod("SetName", &Animal::SetName);
    MReflection::AddClass<Dog>()
        .AddBaseClass<Animal>()
        .AddField("Breed", &Dog::Breed)
        .AddMethod("Speak", &Dog::Speak)
        .AddMethod("Eat", &Dog::Eat);
    auto dogFields = registry.GetType("Dog")->GetFields();
    for (const auto &field : dogFields)
    {
        GTEST_LOG_(INFO) << "Dog Field: " << field->GetName();
    }
    auto dogMethods = registry.GetType("Dog")->GetMethods();
    for (const auto &method : dogMethods)
    {
        GTEST_LOG_(INFO) << "Dog Method: " << method->GetName();
    }
}
TEST(ReflectionTest, ConstructorInvocation)
{
    MReflection::Registry &registry = MReflection::Registry::GetInstance();
    MReflection::AddClass<Dog>()
        .AddConstructor<std::string, int, std::string>("DogConstructor")
        .AddConstructor<std::string>("Ctor2")
        .AddField("Name", &Dog::Name)
        .AddField("Age", &Dog::Age)
        .AddMethod("Speak", &Dog::Speak)
        .AddMethod("CelebrateBirthday", &Dog::CelebrateBirthday);
    auto constructorInfo = registry.GetType<Dog>()->GetConstructor("DogConstructor");
    std::string name = "ReflectedDog";
    std::string breed = "Beagle";
    auto dogInstance = constructorInfo->Create<Dog>(name, 4, breed);
    EXPECT_EQ(dogInstance->Name, "ReflectedDog");
    EXPECT_EQ(dogInstance->Age, 4);

    auto constructorInfo2 = registry.GetType<Dog>()->GetConstructor("Ctor2");
    std::string name2 = "SecondDog";
    auto dogInstance2 = constructorInfo2->Create<Dog>(name2);
    EXPECT_EQ(dogInstance2->Name, "SecondDog");
    EXPECT_EQ(dogInstance2->Age, 0);
    EXPECT_EQ(dogInstance2->Breed, "Unknown");
}