
#include "TypeInfoBuilder.hpp"
#include "MReflection.hpp"
namespace MReflection
{

TypeInfoBuilder::~TypeInfoBuilder()
{
    Registry::GetInstance().mTypes[mTypeInfo->GetName()] = std::move(mTypeInfo);
}
} // namespace MReflection