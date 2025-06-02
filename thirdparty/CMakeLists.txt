include(FetchContent)

add_library(glm INTERFACE)
target_include_directories(glm SYSTEM INTERFACE glm/include)

add_library(sol INTERFACE)
target_include_directories(sol SYSTEM INTERFACE sol/include)

add_library(entt INTERFACE)
target_include_directories(entt SYSTEM INTERFACE entt/include)

add_library(rapidhash INTERFACE)
target_include_directories(rapidhash SYSTEM INTERFACE rapidhash/include)

add_library(glad
        glad/src/glad.c)
target_include_directories(glad SYSTEM PUBLIC glad/include)

add_library(stb_image
        stb_image/src/stb_image.cpp)
target_include_directories(stb_image SYSTEM PUBLIC stb_image/include)

set(IMGUI_SRC_DIR imgui/include/imgui)
add_library(
        imgui
        ${IMGUI_SRC_DIR}/imgui.cpp
        ${IMGUI_SRC_DIR}/imgui_draw.cpp
        ${IMGUI_SRC_DIR}/imgui_impl_opengl3.cpp
        ${IMGUI_SRC_DIR}/imgui_impl_sdl3.cpp
        ${IMGUI_SRC_DIR}/imgui_stdlib.cpp
        ${IMGUI_SRC_DIR}/imgui_tables.cpp
        ${IMGUI_SRC_DIR}/imgui_widgets.cpp
        ${IMGUI_SRC_DIR}/imgui_demo.cpp
)
target_include_directories(imgui SYSTEM PUBLIC imgui/include)
target_link_sdl(imgui)

FetchContent_Declare(
        spdlog
        SOURCE_DIR ${PROJECT_SOURCE_DIR}/thirdparty/spdlog
        SYSTEM
)
FetchContent_MakeAvailable(spdlog)

FetchContent_Declare(
        yaml-cpp
        SOURCE_DIR ${PROJECT_SOURCE_DIR}/thirdparty/yaml-cpp
        SYSTEM
)
FetchContent_MakeAvailable(yaml-cpp)