cmake_minimum_required(VERSION 3.20)

# Project name
project(snek_game)

# Build options
## Set the build type to Release by default
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

## Check whether the build type is valid
set(VALID_BUILD_TYPES Debug Release)

if(NOT CMAKE_BUILD_TYPE IN_LIST VALID_BUILD_TYPES)
    message(FATAL_ERROR "Invalid build type: ${CMAKE_BUILD_TYPE}. Valid options are: ${VALID_BUILD_TYPES}")
endif()

# Compiler settings
## Set C++ standard to C++23
set(COMPILER_FEATURES
    cxx_std_23
)

## Set compiler flags based on the build type
set(DEBUG_FLAGS
    $<$<CXX_COMPILER_ID:GNU,Clang>: -Wall -Wextra -Wpedantic -Werror -g -O0>
    $<$<CXX_COMPILER_ID:MSVC>: /Wall /WX /Zi /Od>
)

set(RELEASE_FLAGS
    $<$<CXX_COMPILER_ID:GNU,Clang>: -O3>
    $<$<CXX_COMPILER_ID:MSVC>: /O2>
)

# Source files & include directories
## Define source and include directories
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(INC_DIR ${CMAKE_SOURCE_DIR}/inc)

## Declare source files
set(SOURCES
    ${SRC_DIR}/main.cpp
)

# External dependencies
## FetchContent module for managing external dependencies
include(FetchContent)

## Fetch SFML library
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.2
    GIT_SHALLOW TRUE
    EXCLUDE_FROM_ALL
    SYSTEM
)
FetchContent_MakeAvailable(SFML)

# Executable target
add_executable(${PROJECT_NAME})

## source and header files
target_sources(${PROJECT_NAME} PRIVATE ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE ${INC_DIR})

## compile flags
target_compile_options(${PROJECT_NAME} PRIVATE
    $<$<CONFIG:Debug>:${DEBUG_FLAGS}>
    $<$<CONFIG:Release>:${RELEASE_FLAGS}>)

## Set the c++ version and disable compiler-specific extensions
target_compile_features(${PROJECT_NAME} PRIVATE ${COMPILER_FEATURES})
set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

## Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    SFML::Window
    SFML::Graphics
    SFML::Audio
)
