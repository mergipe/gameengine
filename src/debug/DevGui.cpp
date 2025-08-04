#include "DevGui.h"
#include "game/Game.h"
#include <imgui/imgui.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_sdl3.h>

namespace Engine
{
    void showMetricsOverlay(bool* show);

    DevGuiImpl::DevGuiImpl(const Window& window)
        : DevGui{}
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

    DevGuiImpl::~DevGuiImpl()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplSDL3_Shutdown();
        ImGui::DestroyContext();
    }

    void DevGuiImpl::processEvent(const SDL_Event& event) { ImGui_ImplSDL3_ProcessEvent(&event); }

    bool DevGuiImpl::wantCaptureKeyboard() { return ImGui::GetIO().WantCaptureKeyboard; }

    bool DevGuiImpl::wantCaptureMouse() { return ImGui::GetIO().WantCaptureMouse; }

    void DevGuiImpl::newFrame()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();
    }

    void DevGuiImpl::render()
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

    void DevGuiImpl::show()
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
