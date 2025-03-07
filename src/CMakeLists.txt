add_executable(gameengine
               Main.cpp
               assets/AssetManager.cpp
               core/IO.cpp core/Logger.cpp core/Timer.cpp core/Window.cpp
               game/Game.cpp game/LevelLoader.cpp game/Scene.cpp game/Systems.cpp
               renderer/Font.cpp renderer/Renderer.cpp renderer/Texture.cpp
               tools/DeveloperModeGui.cpp)
target_compile_features(gameengine PRIVATE cxx_std_23)
set_target_properties(gameengine PROPERTIES
                      CXX_EXTENSIONS OFF
                      RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
target_include_directories(gameengine PUBLIC .)
target_link_sdl(gameengine)
target_link_libraries(gameengine PRIVATE SDL3_image::SDL3_image SDL3_ttf::SDL3_ttf)
target_link_libraries(gameengine PRIVATE glm sol imgui entt)
target_link_libraries(gameengine PRIVATE spdlog::spdlog $<$<BOOL:${MINGW}>:ws2_32>)
target_link_libraries(gameengine PRIVATE ${LUA_LIBRARIES})

if (ENABLE_DEVELOPER_MODE)
    message("Setting developer mode compiler flags")
    target_compile_options(gameengine PRIVATE -Wall -Wextra -Werror -Wconversion -Wsign-conversion -Weffc++)
endif()
