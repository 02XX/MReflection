#include "Generator.hpp"

namespace MReflection
{
Generator::Generator()
{
    mClangIndex = clang_createIndex(0, 0);
    mFileTemplate = R"(#pragma once
#include "Registry.hpp"
{{ content }}
)";
    mClassTemplate = R"(class {{ class_name }}_Reflection
{
    public:
    {{ class_name }}_Reflection()
    {
        // Reflection code for {{ class_name }}
        MReflection::AddClass("{{ class_name }}")
        .AddBaseClass("{{ base_class_name }}")
        {% for field in fields %}.AddField("{{ field }}", &{{class_name}}::{{ field }}){% endfor %}
        {% for method in methods %}.AddMethod("{{ method }}", &{{class_name}}::{{ method }}){% endfor %};
    }
};
static {{ class_name }}_Reflection {{ class_name }}_reflection_instance;)";
}

Generator::~Generator()
{
    clang_disposeIndex(mClangIndex);
}
void Generator::Generate(const std::filesystem::path &sourceDirectory)
{
    if (!std::filesystem::exists(sourceDirectory) || !std::filesystem::is_directory(sourceDirectory))
    {
        throw std::runtime_error("Source directory does not exist or is not a directory: " + sourceDirectory.string());
    }
    auto parentDirectory = sourceDirectory.parent_path();
    auto outputDirectory = parentDirectory / "generated";
    std::filesystem::create_directory(outputDirectory);
    for (const auto &entry : std::filesystem::directory_iterator(sourceDirectory))
    {
        if (entry.path().extension() == ".hpp" || entry.path().extension() == ".h")
        {
            auto outputPath = outputDirectory / std::format("{}_Reflection.hpp", entry.path().stem().string());
            Generate(entry.path(), outputPath);
            std::cout << "Generated: " << outputPath << std::endl;
        }
    }
    std::cout << "Generated files are located in: " << outputDirectory << std::endl;
}
void Generator::Generate(const std::filesystem::path &sourcePath, const std::filesystem::path &outputPath)
{
    if (!std::filesystem::exists(sourcePath))
    {
        throw std::runtime_error("Source file does not exist: " + sourcePath.string());
    }
    auto translationUnit = clang_parseTranslationUnit(mClangIndex, sourcePath.string().c_str(), nullptr, 0, nullptr, 0,
                                                      CXTranslationUnit_None);
    auto rootCursor = clang_getTranslationUnitCursor(translationUnit);
    auto reflectClasses = GetReflectClasses(rootCursor);
    if (!reflectClasses.empty())
    {
        std::string content{};
        for (const auto &classCursor : reflectClasses)
        {
            auto meta = GetMeta(classCursor);
            content += mInjaEnv.render(mClassTemplate, meta.to_json()) + "\n";
        }
        std::fstream outputFile;
        outputFile.open(outputPath, std::ios::out);
        outputFile << mInjaEnv.render(mFileTemplate, {{"content", content}});
        outputFile.close();
    }
    clang_disposeTranslationUnit(translationUnit);
}
std::vector<CXCursor> Generator::GetReflectClasses(CXCursor rootCursor) const
{
    std::vector<CXCursor> reflectClasses;
    clang_visitChildren(
        rootCursor,
        [](CXCursor cursor, CXCursor parent, CXClientData clientData) {
            auto &reflectClasses = *static_cast<std::vector<CXCursor> *>(clientData);
            Generator self;
            if (clang_getCursorKind(cursor) == CXCursor_ClassDecl && self.IsReflectClass(cursor))
            {
                reflectClasses.push_back(cursor);
            }
            return CXChildVisit_Recurse;
        },
        &reflectClasses);
    return reflectClasses;
}
Meta Generator::GetMeta(CXCursor classCursor)
{
    Meta meta;
    CXString className = clang_getCursorSpelling(classCursor);
    meta.ClassName = clang_getCString(className);
    clang_disposeString(className);

    clang_visitChildren(
        classCursor,
        [](CXCursor cursor, CXCursor parent, CXClientData clientData) {
            auto &meta = *static_cast<Meta *>(clientData);
            CXCursorKind kind = clang_getCursorKind(cursor);
            if (kind == CXCursor_CXXBaseSpecifier)
            {
                CXCursor baseCursor = clang_getCursorReferenced(cursor);
                CXString baseName = clang_getCursorSpelling(baseCursor);
                meta.BaseClassName = clang_getCString(baseName);
                clang_disposeString(baseName);
            }
            else if (kind == CXCursor_FieldDecl)
            {
                CXString fieldName = clang_getCursorSpelling(cursor);
                meta.Fields.push_back(clang_getCString(fieldName));
                clang_disposeString(fieldName);
            }
            else if (kind == CXCursor_CXXMethod)
            {
                CXString methodName = clang_getCursorSpelling(cursor);
                meta.Methods.push_back(clang_getCString(methodName));
                clang_disposeString(methodName);
            }
            return CXChildVisit_Continue;
        },
        &meta);
    return meta;
}
bool Generator::IsReflectClass(CXCursor cursor) const
{
    auto baseClasses = GetBaseClasses(cursor);
    for (const auto &base : baseClasses)
    {
        CXString baseName = clang_getCursorSpelling(base);
        if (strcmp(clang_getCString(baseName), "MObject") == 0)
        {
            clang_disposeString(baseName);
            return true;
        }
        clang_disposeString(baseName);
    }
    // 递归检查基类的基类
    for (const auto &base : baseClasses)
    {
        if (IsReflectClass(base))
        {
            return true;
        }
    }
    return false;
}
std::vector<CXCursor> Generator::GetBaseClasses(CXCursor classCursor) const
{
    if (clang_getCursorKind(classCursor) != CXCursor_ClassDecl)
    {
        return {};
    }
    std::vector<CXCursor> baseClasses;
    clang_visitChildren(
        classCursor,
        [](CXCursor cursor, CXCursor parent, CXClientData clientData) {
            auto &bases = *static_cast<std::vector<CXCursor> *>(clientData);
            if (clang_getCursorKind(cursor) == CXCursor_CXXBaseSpecifier)
            {
                auto self = static_cast<Generator *>(clientData);
                CXCursor baseCursor = clang_getCursorReferenced(cursor);
                if (clang_isDeclaration(clang_getCursorKind(baseCursor)))
                {
                    bases.push_back(baseCursor);
                    self->GetBaseClasses(baseCursor);
                }
            }
            return CXChildVisit_Continue;
        },
        &baseClasses);
    return baseClasses;
}
} // namespace MReflection