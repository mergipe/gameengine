add_executable(gameengine
               Main.cpp
               core/Logger.cpp core/IO.cpp core/Renderer.cpp core/Timer.cpp core/Window.cpp
               game/Game.cpp game/ECS.cpp game/Systems.cpp
               resources/ResourceManager.cpp resources/Texture.cpp)
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
