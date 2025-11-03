#pragma once
#include <string>

namespace MReflection
{
class TypeInfo;
class MemberInfo
{
    friend class TypeInfoBuilder;

  protected:
    MemberInfo(const std::string &name) : mName(name)
    {
    }

  public:
    inline const std::string &GetName() const
    {
        return mName;
    }

  private:
    std::string mName;
};
} // namespace MReflection