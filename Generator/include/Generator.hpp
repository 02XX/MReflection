#pragma once
#include "Meta.hpp"
#include <clang-c/Index.h>
#include <filesystem>
#include <inja/inja.hpp>
#include <vector>

namespace MReflection
{
class Generator
{
  private:
    CXIndex mClangIndex;
    std::string mFileTemplate;
    std::string mClassTemplate;
    inja::Environment mInjaEnv;

    bool IsReflectClass(CXCursor cursor) const;
    std::vector<CXCursor> GetBaseClasses(CXCursor classCursor) const;

  public:
    Generator();
    ~Generator();
    std::vector<CXCursor> GetReflectClasses(CXCursor rootCursor) const;
    Meta GetMeta(CXCursor classCursor);

    void Generate(const std::filesystem::path &sourceDirectory);
    void Generate(const std::filesystem::path &sourcePath, const std::filesystem::path &outputPath);
};
} // namespace MReflection
