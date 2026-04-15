include(FetchContent)
include(ExternalProject)

set(LUAJIT_INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}/luajit_install)
ExternalProject_Add(luajit_build
        SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/luajit
        CONFIGURE_COMMAND ""
        BUILD_COMMAND make
        INSTALL_COMMAND make install PREFIX=${LUAJIT_INSTALL_DIR}
        BUILD_IN_SOURCE 1
)
add_library(luajit STATIC IMPORTED GLOBAL)
set(LUAJIT_LIB_PATH "${LUAJIT_INSTALL_DIR}/lib/libluajit-5.1.a")
set(LUAJIT_INC_DIR "${LUAJIT_INSTALL_DIR}/include/luajit-2.1")
set_target_properties(luajit PROPERTIES
        IMPORTED_LOCATION ${LUAJIT_LIB_PATH}
        INTERFACE_INCLUDE_DIRECTORIES ${LUAJIT_INC_DIR}
)
add_dependencies(luajit luajit_build)

FetchContent_Declare(
        SDL
        SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/SDL
        SYSTEM
)
FetchContent_MakeAvailable(SDL)

FetchContent_Declare(
        spdlog
        SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/spdlog
        SYSTEM
)
FetchContent_MakeAvailable(spdlog)

FetchContent_Declare(
        yaml-cpp
        SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/yaml-cpp
        SYSTEM
)
FetchContent_MakeAvailable(yaml-cpp)

FetchContent_Declare(
        box2d
        SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/box2d
        SYSTEM
)
FetchContent_MakeAvailable(box2d)

add_library(glad
        glad/src/glad.c)
target_include_directories(glad SYSTEM PUBLIC glad/include)

add_library(stb_image
        stb_image/src/stb_image.cpp)
target_include_directories(stb_image SYSTEM PUBLIC stb_image/include)

add_library(
        imgui
        imgui/imgui.cpp
        imgui/imgui_draw.cpp
        imgui/imgui_impl_opengl3.cpp
        imgui/imgui_impl_sdl3.cpp
        imgui/imgui_stdlib.cpp
        imgui/imgui_tables.cpp
        imgui/imgui_widgets.cpp
        imgui/imgui_demo.cpp
)
target_include_directories(imgui SYSTEM PUBLIC imgui)
target_link_sdl(imgui)

add_library(glm INTERFACE)
target_include_directories(glm SYSTEM INTERFACE glm/include)

add_library(sol INTERFACE)
target_include_directories(sol SYSTEM INTERFACE sol/include)

add_library(entt INTERFACE)
target_include_directories(entt SYSTEM INTERFACE entt/include)

add_library(wyhash INTERFACE)
target_include_directories(wyhash SYSTEM INTERFACE wyhash/include)
