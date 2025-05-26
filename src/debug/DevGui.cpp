#include "DevGui.h"
#include "game/Game.h"
#include <imgui/imgui.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_sdl3.h>

namespace Engine
{
    void showMetricsOverlay(bool* show);

    DevGui::DevGui(const Window& window)
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

    DevGui::~DevGui()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplSDL3_Shutdown();
        ImGui::DestroyContext();
    }

    void DevGui::processEvent(const SDL_Event& event)
    {
        if (event.type == SDL_EVENT_KEY_DOWN && wantCaptureKeyboard()) {
            if (event.key.scancode == Game::s_toggleDevModeKey && !event.key.repeat) {
                Game::instance().toggleDevMode();
            }
        }
        ImGui_ImplSDL3_ProcessEvent(&event);
    }

    bool DevGui::wantCaptureKeyboard() { return ImGui::GetIO().WantCaptureKeyboard; }

    bool DevGui::wantCaptureMouse() { return ImGui::GetIO().WantCaptureMouse; }

    void DevGui::newFrame()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();
    }

    void DevGui::render()
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

    void DevGui::show()
    {
        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("Profile/Debug")) {
                ImGui::MenuItem("Metrics overlay", nullptr, &m_data.showMetricsOverlay);
                ImGui::MenuItem("Metrics/Debugger", nullptr, &m_data.showMetricsWindow);
                ImGui::EndMenu();
            }
            ImGui::MenuItem("ImGui Demo", nullptr, &m_data.showDemoWindow);
            ImGui::EndMainMenuBar();
        }
        if (m_data.showMetricsOverlay) {
            showMetricsOverlay(&m_data.showMetricsOverlay);
        }
        if (m_data.showMetricsWindow) {
            ImGui::ShowMetricsWindow(&m_data.showMetricsWindow);
        }
        if (m_data.showDemoWindow) {
            ImGui::ShowDemoWindow(&m_data.showDemoWindow);
        }
    }

    void showMetricsOverlay(bool* show)
    {
        constexpr ImGuiWindowFlags windowFlags{
            ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize |
            ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav};
        ImGui::SetNextWindowBgAlpha(0.35f);
        if (ImGui::Begin("Metrics overlay", show, windowFlags)) {
            ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
            if (ImGui::BeginPopupContextWindow()) {
                if (show && ImGui::MenuItem("Close")) {
                    *show = false;
                }
                ImGui::EndPopup();
            }
            ImGui::End();
        }
    }
} // namespace Engine
