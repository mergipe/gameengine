add_executable(gameengine
               Main.cpp
               core/Config.cpp
               core/Filesystem.cpp
               core/IO.cpp
               core/Logger.cpp
               core/Timer.cpp
               core/Window.cpp
               input/InputHandler.cpp
               game/Game.cpp
               game/Scene.cpp
               game/SceneLoader.cpp
               game/Systems.cpp
               renderer/Camera.cpp
               renderer/Renderer2D.cpp
               renderer/Shader.cpp
               renderer/ShaderManager.cpp
               resources/Font.cpp
               resources/ResourceManager.cpp
               resources/Texture2D.cpp
               tools/DevGui.cpp)
target_compile_features(gameengine PRIVATE cxx_std_23)
set_target_properties(gameengine PROPERTIES
                      CXX_EXTENSIONS OFF
                      RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
target_include_directories(gameengine PUBLIC .)
target_link_sdl(gameengine)
target_link_libraries(gameengine PRIVATE glm sol imgui entt glad stb_image)
target_link_libraries(gameengine PRIVATE spdlog::spdlog $<$<BOOL:${MINGW}>:ws2_32>)
target_link_libraries(gameengine PRIVATE ${LUA_LIBRARIES})
target_link_libraries(gameengine PRIVATE ${OPENGL_LIBRARIES})
target_compile_definitions(gameengine PUBLIC SOL_ALL_SAFETIES_ON=1)

if (ENABLE_DEVELOPER_MODE)
    message("Setting developer mode compiler flags")
    target_compile_options(gameengine PRIVATE -Wall -Wextra -Werror -Wconversion -Wsign-conversion -Weffc++)
endif()
