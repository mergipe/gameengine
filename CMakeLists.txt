cmake_minimum_required(VERSION 3.15)
project(GameEngine LANGUAGES CXX C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
option(ENABLE_DEVELOPER_MODE "Enable developer mode" ON)

set(default_build_type "RelWithDebInfo")
if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
  set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE
      STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
    "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

find_program(CCACHE_PROGRAM ccache)
if (CCACHE_PROGRAM)
    set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
endif()

option(SDL_VENDORED "Use vendored SDL" OFF)
if (SDL_VENDORED)
  add_subdirectory(libs/SDL)
else()
  find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3-shared)
endif()

find_package(OpenGL REQUIRED)
find_package(Lua REQUIRED)

include(cmake/LinkSDL.cmake)
add_subdirectory(libs)
add_subdirectory(src)
