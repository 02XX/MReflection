#include "gtest/gtest.h"
#include <functional>
#include <gtest/gtest.h>
#include <memory>
#include <type_traits>
#include <utility>

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
template <class T> void printType(const T &obj)
{
    GTEST_LOG_(INFO) << "Left Value Type: " << typeid(T).name();
}
template <class T> void printType(T &&obj)
{
    GTEST_LOG_(INFO) << "Right Value Type: " << typeid(T).name();
}
template <class T> void TestFieldType(T &&value)
{
    using U = std::remove_cvref<T>::type;
    if (std::is_reference<U>())
    {
        GTEST_LOG_(INFO) << "Is Reference Type";
    }
    if (std::is_same<U, Animal &>())
    {
        GTEST_LOG_(INFO) << "Is LValue Reference Type";
    }
    if (std::is_same<U, Animal &&>())
    {
        GTEST_LOG_(INFO) << "Is RValue Reference Type";
    }
    if (std::is_same<U, const Animal &>())
    {
        GTEST_LOG_(INFO) << "Is Const LValue Reference Type";
    }
    if (std::is_same<U, const Animal &&>())
    {
        GTEST_LOG_(INFO) << "Is RValue Reference Type";
    }
    if (std::is_same<U, Animal>())
    {
        GTEST_LOG_(INFO) << "Is Value Type";
    }
    auto test = [](std::any obj) { auto o = std::any_cast<U>(std::move(obj)); };
    test(std::forward<T>(value));
}
// TEST(AnyTest, Basic)
// {
//     auto test = [](std::any obj) {
//         const Animal& value = std::any_cast<std::reference_wrapper<const Animal>>(obj).get();
//     };
//     auto test2 = [&test](const Animal &obj) { test(std::cref(obj)); };
//     auto animal = std::make_unique<Animal>(5);
//     test2(*animal);
// }
TEST(AnyTest, TestFieldType)
{
    auto animal = std::make_unique<Animal>(5);
    TestFieldType(*animal);
}
// TEST(AnyTest, any_cast_Test)
// {
//     auto animal = std::make_unique<Animal>(6);
//     Animal *animalPtr = animal.get();
//     const Animal *constAnimalPtr = animal.get();
//     Animal &animalRef = *animal;
//     const Animal &constAnimalRef = *animal;
//     auto printType = [](std::any obj) { GTEST_LOG_(INFO) << "Type: " << obj.type().name(); };
//     // {
//     //     printRType(animalPtr);
//     //     printRType(constAnimalPtr);
//     //     printRType(animalRef);
//     //     printRType(constAnimalRef);
//     // }
//     // {
//     //     printType(animalPtr);
//     //     printType(constAnimalPtr);
//     //     printType(animalRef);
//     //     printType(constAnimalRef);
//     //     printType(std::ref(*animalPtr));
//     //     printType(std::cref(*animalPtr));
//     // }
//     {

//         // int age = 10;
//         // int &ageRef = age;
//         // int *agePtr = &age;
//         // func(animalPtr, 10);
//         // func(animalPtr, age);
//         // func(animalPtr, ageRef);
//         // func(animalPtr, agePtr);
//         // GetValue<int>(animalRef);
//         // GetValue<int>(constAnimalRef);
//         // GetValue<int>(animalPtr);
//         // GetValue<int>(constAnimalPtr);
//         // SetValue(animalRef, 10);
//         // SetValue(constAnimalRef, 10);
//         // SetValue(animalPtr, 10);
//         // SetValue(constAnimalPtr, 10);
//     }
//     {

//     }
// }