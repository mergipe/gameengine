add_executable(${PROJECT_NAME}
        Main.cpp
        core/Config.cpp
        core/Filesystem.cpp
        core/IO.cpp
        core/Logger.cpp
        core/StringId.cpp
        core/Timer.cpp
        core/Window.cpp
        debug/DevGui.cpp
        input/InputHandler.cpp
        game/Entity.cpp
        game/Game.cpp
        game/Scene.cpp
        game/SceneLoader.cpp
        game/Script.cpp
        game/Systems.cpp
        renderer/Camera.cpp
        renderer/Renderer2D.cpp
        renderer/Shader.cpp
        renderer/ShaderManager.cpp
        resources/Font.cpp
        resources/ResourceManager.cpp
        resources/Texture2D.cpp)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)
set_target_properties(${PROJECT_NAME} PROPERTIES
        CXX_EXTENSIONS OFF
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC .)
target_link_sdl(${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} PRIVATE glm sol imgui entt glad stb_image rapidhash spdlog yaml-cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE ${LUA_LIBRARIES})
target_link_libraries(${PROJECT_NAME} PRIVATE ${OPENGL_LIBRARIES})
target_compile_definitions(${PROJECT_NAME} PUBLIC SOL_ALL_SAFETIES_ON=1)

if (ENABLE_DEVELOPER_MODE)
    message("Setting developer mode compiler flags")
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Werror -Wconversion -Wsign-conversion -Weffc++)
endif ()
