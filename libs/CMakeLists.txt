add_library(glm INTERFACE)
target_include_directories(glm SYSTEM INTERFACE glm/include)

add_library(sol INTERFACE)
target_include_directories(sol SYSTEM INTERFACE sol/include)

set(IMGUI_SRC_DIR imgui/include/imgui)
add_library(
    imgui
    ${IMGUI_SRC_DIR}/imgui.cpp
    ${IMGUI_SRC_DIR}/imgui_draw.cpp
    ${IMGUI_SRC_DIR}/imgui_impl_sdl2.cpp
    ${IMGUI_SRC_DIR}/imgui_impl_sdlrenderer2.cpp
    ${IMGUI_SRC_DIR}/imgui_tables.cpp
    ${IMGUI_SRC_DIR}/imgui_widgets.cpp
    ${IMGUI_SRC_DIR}/imgui_demo.cpp
)
target_include_directories(imgui SYSTEM PUBLIC imgui/include)
link_sdl2(imgui)
