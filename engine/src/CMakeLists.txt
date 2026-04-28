add_executable(${PROJECT_NAME}
        Main.cpp
        Engine.cpp
        core/Config.cpp
        core/Filesystem.cpp
        core/IO.cpp
        core/Logger.cpp
        core/StringId.cpp
        core/Timer.cpp
        core/Window.cpp
        debug/DevGui.cpp
        input/InputCallback.cpp
        input/InputCommandMapping.cpp
        input/InputConfigLoader.cpp
        input/InputHandler.cpp
        input/InputScope.cpp
        physics/2d/Body2D.cpp
        physics/2d/PhysicsEngine2D.cpp
        renderer/Camera.cpp
        renderer/Renderer2D.cpp
        renderer/Shader.cpp
        renderer/ShaderManager.cpp
        resources/Font.cpp
        resources/ResourceManager.cpp
        resources/Texture2D.cpp
        scene/Entity.cpp
        scene/Scene.cpp
        scene/SceneLoader.cpp
        scene/ScriptInstance.cpp
        scene/Systems.cpp
)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)
set_target_properties(${PROJECT_NAME} PROPERTIES
        CXX_EXTENSIONS OFF
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC .)
add_dependencies(${PROJECT_NAME} luajit_build)
target_link_sdl(${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} PRIVATE box2d entt glad glm imgui luajit sol spdlog stb_image wyhash yaml-cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE ${OPENGL_LIBRARIES})
target_compile_definitions(${PROJECT_NAME} PRIVATE SOL_ALL_SAFETIES_ON=1 SOL_LUAJIT=1 ENGINE_BASE_DIR="${CMAKE_SOURCE_DIR}")

if (ENABLE_DEVELOPER_MODE)
    message("Setting developer mode compiler flags")
    target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CXX_COMPILER_ID:GNU,Clang>:
            -Wall
            -Wextra
            -Werror
            -Wpedantic
            -Wconversion
            -Wsign-conversion
            -Wno-error=array-bounds
            >
            $<$<CXX_COMPILER_ID:MSVC>:
            /W4
            /WX
            /w44365
            /external:W3
            >
    )
endif ()
