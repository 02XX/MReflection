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
    const int id = 1;
    int Age;
};

class Dog : public Animal
{
  public:
    Dog(const std::string &name, int age, const std::string &breed) : Animal(name, age), Breed(breed)
    {
    }
    std::string Breed;
};
TEST(FieldTest, Getter_Reference)
{
    auto animal = std::make_shared<Animal>("GenericAnimal", 5);
    FieldInfo nameField("Name", &Animal::Name, nullptr);
    FieldInfo ageField("Age", &Animal::Age, nullptr);
    EXPECT_EQ(nameField.GetValue<std::string>(*animal), "GenericAnimal");
    EXPECT_EQ(ageField.GetValue<int>(*animal), 5);
}
TEST(FieldTest, Setter_Reference)
{
    auto animal = std::make_shared<Animal>("GenericAnimal", 5);
    FieldInfo nameField("Name", &Animal::Name, nullptr);
    FieldInfo ageField("Age", &Animal::Age, nullptr);
    nameField.SetValue<std::string>(*animal, "UpdatedAnimal");
    ageField.SetValue<int>(*animal, 6);
    EXPECT_EQ(animal->Name, "UpdatedAnimal");
    EXPECT_EQ(animal->Age, 6);
}
TEST(FieldTest, Getter_Pointer)
{
    auto animal = std::make_shared<Animal>("GenericAnimal", 5);
    FieldInfo nameField("Name", &Animal::Name, nullptr);
    FieldInfo ageField("Age", &Animal::Age, nullptr);
    EXPECT_EQ(nameField.GetValue<std::string>(animal.get()), "GenericAnimal");
    EXPECT_EQ(ageField.GetValue<int>(animal.get()), 5);
}
TEST(FieldTest, Setter_Pointer)
{
    auto animal = std::make_shared<Animal>("GenericAnimal", 5);
    FieldInfo nameField("Name", &Animal::Name, nullptr);
    FieldInfo ageField("Age", &Animal::Age, nullptr);
    nameField.SetValue<std::string>(animal.get(), "UpdatedAnimal");
    ageField.SetValue<int>(animal.get(), 6);
    EXPECT_EQ(animal->Name, "UpdatedAnimal");
    EXPECT_EQ(animal->Age, 6);
}
TEST(FieldTest, Getter_SharedPointer)
{
    auto animal = std::make_shared<Animal>("GenericAnimal", 5);
    FieldInfo nameField("Name", &Animal::Name, nullptr);
    FieldInfo ageField("Age", &Animal::Age, nullptr);
    EXPECT_EQ(nameField.GetValue<std::string>(animal), "GenericAnimal");
    EXPECT_EQ(ageField.GetValue<int>(animal), 5);
}
TEST(FieldTest, Setter_SharedPointer)
{
    auto animal = std::make_shared<Animal>("GenericAnimal", 5);
    FieldInfo nameField("Name", &Animal::Name, nullptr);
    FieldInfo ageField("Age", &Animal::Age, nullptr);
    nameField.SetValue<std::string>(animal, "UpdatedAnimal");
    ageField.SetValue<int>(animal, 6);
    EXPECT_EQ(animal->Name, "UpdatedAnimal");
    EXPECT_EQ(animal->Age, 6);
}

TEST(FieldTest, Getter_ConstField)
{
    auto animal = std::make_shared<Animal>("GenericAnimal", 5);
    FieldInfo idField("id", &Animal::id, nullptr);
    EXPECT_EQ(idField.GetValue<int>(*animal), 1);
}
TEST(FieldTest, Setter_ConstField)
{
    auto animal = std::make_shared<Animal>("GenericAnimal", 5);
    FieldInfo idField("id", &Animal::id, nullptr);
    EXPECT_THROW(idField.SetValue<int>(*animal, 10), std::runtime_error);
}