cmake_minimum_required(VERSION 3.15)
project(GameEngine LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(default_build_type "RelWithDebInfo")
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
  set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE
      STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
    "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

find_package(SDL2 REQUIRED CONFIG REQUIRED COMPONENTS SDL2)
find_package(SDL2 REQUIRED CONFIG COMPONENTS SDL2main)
find_package(SDL2_image REQUIRED CONFIG)
find_package(SDL2_ttf REQUIRED CONFIG)
find_package(SDL2_mixer REQUIRED CONFIG)
if (NOT TARGET spdlog)
    set(spdlog_DIR libs/spdlog/build)
    find_package(spdlog REQUIRED)
endif()

add_subdirectory(libs)
add_subdirectory(src)
