#include "FieldInfo.hpp"
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

TEST(FieldTest, GetSetField)
{
    auto animal = std::make_shared<Animal>("GenericAnimal", 5);
    FieldInfo nameField("Name", &Animal::Name, nullptr);
    FieldInfo ageField("Age", &Animal::Age, nullptr);
    std::any nameValue = nameField.GetValue<std::string>(*animal);
    EXPECT_EQ(std::any_cast<std::string>(nameValue), "GenericAnimal");
    std::any ageValue = ageField.GetValue<int>(*animal);
    EXPECT_EQ(std::any_cast<int>(ageValue), 5);

    nameField.SetValue(*animal, std::string("NewAnimalName"));
    ageField.SetValue(*animal, 10);
    EXPECT_EQ(animal->Name, "NewAnimalName");
    EXPECT_EQ(animal->Age, 10);
}
