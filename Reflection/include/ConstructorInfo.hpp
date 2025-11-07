#pragma once
#include "MemberInfo.hpp"
#include "MemberTypes.hpp"
#include <any>
#include <functional>
#include <memory>

namespace MReflection
{
class ConstructorInfo : public MemberInfo
{
    using InvokeHandler = std::function<std::any(std::any, std::any)>;

  public:
    ConstructorInfo(const std::string &name) : MemberInfo(name)
    {
    }
    template <class TClass, typename... TArgs> std::shared_ptr<TClass> Invoke(TArgs &&...args) const
    {
        if constexpr (sizeof...(TArgs) == 0)
        {
            // Default constructor
            return std::make_shared<TClass>();
        }
        else
        {
            return std::make_shared<TClass>(std::forward<TArgs>(args)...);
        }
    }

  private:
    MemberType mMemberType{MemberType::Constructor};
    InvokeHandler mInvokeHandler;
};
} // namespace MReflection