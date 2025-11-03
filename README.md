# MReflection

一个运行时反射框架，风格类似C#

## TODO

- [x] 基本运行时反射功能
- [x] 继承支持
- [ ] 构造反射
- [ ] clang AST自动生成反射元信息

## 快速开始

```CPP
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
MReflection::AddTClass("Animal")
    .AddField("Name", &Animal::Name)
    .AddField("Age", &Animal::Age)
    .AddMethod("Speak", &Animal::Speak)
    .AddMethod("CelebrateBirthday", &Animal::CelebrateBirthday);
MReflection::Registry &registry = MReflection::Registry::GetInstance();
auto nameField = registry.GetType("Animal")->GetField("Name");
nameField->SetValue<std::string>(*animal, "NewAnimalName");
auto celebrateMethod = registry.GetType("Animal")->GetMethod("CelebrateBirthday");
celebrateMethod->Invoke(*animal);
//or
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
```

## 如何贡献

1. 提交 [Issue](https://github.com/02XX/MEngineV4/issues) 说明问题或建议
2. Fork 项目并创建功能分支
3. 确保代码符合 clang-format 规范
4. 提交 Pull Request 并关联 Issue

## 许可证

GNU General Public License v3. - 详见[LICENSE](LICENSE)文件
