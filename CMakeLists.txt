cmake_minimum_required(VERSION 3.20.0)
project(MReflection VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(Reflection)

enable_testing()
add_subdirectory(Test)
