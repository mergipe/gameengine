include(FetchContent)

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

add_library(
        lua
        lua/lapi.c
        lua/lauxlib.c
        lua/lbaselib.c
        lua/lcode.c
        lua/lcorolib.c
        lua/lctype.c
        lua/ldblib.c
        lua/ldebug.c
        lua/ldo.c
        lua/ldump.c
        lua/lfunc.c
        lua/lgc.c
        lua/linit.c
        lua/liolib.c
        lua/llex.c
        lua/lmathlib.c
        lua/lmem.c
        lua/loadlib.c
        lua/lobject.c
        lua/lopcodes.c
        lua/loslib.c
        lua/lparser.c
        lua/lstate.c
        lua/lstring.c
        lua/lstrlib.c
        lua/ltable.c
        lua/ltablib.c
        lua/ltm.c
        lua/lundump.c
        lua/lutf8lib.c
        lua/lvm.c
        lua/lzio.c
)
target_include_directories(lua SYSTEM PUBLIC lua)

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
