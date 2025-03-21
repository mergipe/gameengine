#include "DeveloperModeGui.h"
#include <imgui/imgui.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_sdl3.h>

namespace Engine
{
    void newFrame()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();
    }

    void render()
    {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            SDL_Window* currentWindow{SDL_GL_GetCurrentWindow()};
            SDL_GLContext currentContext{SDL_GL_GetCurrentContext()};
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            SDL_GL_MakeCurrent(currentWindow, currentContext);
        }
    }

    DeveloperModeGui::DeveloperModeGui(const Window& window)
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io{ImGui::GetIO()};
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        ImFontConfig config{};
        config.SizePixels = 20.0f;
        io.Fonts->AddFontDefault(&config);
        ImGui::StyleColorsDark();
        ImGuiStyle& style{ImGui::GetStyle()};
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }
        style.ScaleAllSizes(2.0f);
        ImGui_ImplSDL3_InitForOpenGL(window.getWindowPtr(), window.getGLContext());
        ImGui_ImplOpenGL3_Init("#version 330 core");
    }

    DeveloperModeGui::~DeveloperModeGui()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplSDL3_Shutdown();
        ImGui::DestroyContext();
    }

    void DeveloperModeGui::processEvent(SDL_Event& event) { ImGui_ImplSDL3_ProcessEvent(&event); }

    void DeveloperModeGui::show()
    {
        newFrame();
        ImGui::ShowDemoWindow();
        render();
    }
} // namespace Engine
