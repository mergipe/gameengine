add_executable(gameengine
               Main.cpp
               core/IO.cpp core/Logger.cpp core/Timer.cpp core/Window.cpp
               game/ECS.cpp game/Game.cpp game/Scene.cpp game/Systems.cpp
               renderer/Renderer.cpp renderer/Texture.cpp
               resources/ResourceManager.cpp)
target_compile_features(gameengine PRIVATE cxx_std_23)
set_target_properties(gameengine PROPERTIES
                      CXX_EXTENSIONS OFF
                      RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
target_include_directories(gameengine PUBLIC .)
target_link_sdl2(gameengine)
target_link_libraries(gameengine PRIVATE SDL2_image::SDL2_image SDL2_ttf::SDL2_ttf SDL2_mixer::SDL2_mixer)
target_link_libraries(gameengine PRIVATE glm sol imgui)
target_link_libraries(gameengine PRIVATE spdlog::spdlog $<$<BOOL:${MINGW}>:ws2_32>)
target_link_libraries(gameengine PRIVATE ${LUA_LIBRARIES})

if (ENABLE_DEVELOPER_MODE)
    message("Setting developer mode compiler flags")
    target_compile_options(gameengine PRIVATE -Wall -Wextra -Werror -Wconversion -Wsign-conversion -Weffc++)
endif()
