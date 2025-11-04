file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp")
find_path(libclang_INCLUDE NAMES clang-c/Index.h REQUIRED)
find_library(libclang_LIB NAMES clang REQUIRED)
 find_package(inja CONFIG REQUIRED)

add_library(Generator ${SOURCES})

target_include_directories(Generator PUBLIC "include" ${libclang_INCLUDE})
target_link_libraries(Generator PUBLIC ${libclang_LIB} pantor::inja)

