file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp")

add_library(MReflection ${SOURCES})

target_include_directories(MReflection PUBLIC "include")

