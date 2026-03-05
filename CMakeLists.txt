cmake_minimum_required(VERSION 3.24)
project(My_PPP)

# set up the C++ standard to C++11
set(CMAKE_CXX_STANDARD 11)
# enforce the C++ standard requirement
set(CMAKE_CXX_STANDARD_REQUIRED ON) 
# let cmake generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(${CMAKE_SOURCE_DIR}/include)  # make header files relative to the root directory

if(MSVC)
    # Windows for MSVC
    add_compile_options(/W4 /WX)
    add_compile_options(/utf-8)
else()
    # GCC and Clang
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

# add the sub_directory for each chapter
file(GLOB CHAPTERS RELATIVE ${CMAKE_SOURCE_DIR} chapter*)
foreach(c IN ITEMS ${CHAPTERS})
    add_subdirectory(${c})
endforeach()

