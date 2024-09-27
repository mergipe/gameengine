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
target_link_sdl2(imgui)

execute_process(COMMAND ${CMAKE_COMMAND} -S spdlog -B spdlog/build
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
execute_process(COMMAND make -j
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/spdlog/build)
if (NOT TARGET spdlog)
    set(spdlog_DIR spdlog/build)
    find_package(spdlog REQUIRED GLOBAL)
endif()
