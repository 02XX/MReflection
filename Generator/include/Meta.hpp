#pragma once
#include <inja/inja.hpp>
#include <string>
#include <vector>

namespace MReflection
{
struct Meta
{
    std::string ClassName;
    std::string BaseClassName;
    std::vector<std::string> Fields;
    std::vector<std::string> Methods;
    inja::json to_json() const
    {
        inja::json j;
        j["class_name"] = ClassName;
        j["base_class_name"] = BaseClassName;
        j["fields"] = Fields;
        j["methods"] = Methods;
        return j;
    }
};
} // namespace MReflection