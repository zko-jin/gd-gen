cmake_minimum_required(VERSION 3.17)
project(gd-gen LANGUAGES CXX)

file(GLOB_RECURSE MY_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_executable(gd-gen ${MY_SRC})
target_include_directories(gd-gen PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
set_target_properties(gd-gen PROPERTIES 
RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} #place .exe right here in the gd-gen folder
CXX_STANDARD 20 #Using C++20
)
target_compile_options(gd-gen PRIVATE #Is this needed?
    $<$<CONFIG:Release>:-O3 -Ofast>
    $<$<CONFIG:Debug>:-g -O0 -Wall>
)
target_link_options(gd-gen PRIVATE $<$<CONFIG:Release>:-s>) #Strip symbols in release mode