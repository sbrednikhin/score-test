cmake_minimum_required(VERSION 3.27)
project(sw_battle_test)

set(CMAKE_CXX_STANDARD 20)

# Включаем отладочные логи в Debug сборке
#if(CMAKE_BUILD_TYPE STREQUAL "Debug")
#    add_definitions(-DDEBUG_BUILD=1)
#endif()

file(GLOB_RECURSE SOURCES src/*.cpp src/*.hpp)
add_executable(sw_battle_test ${SOURCES})

target_include_directories(sw_battle_test PUBLIC src/)
