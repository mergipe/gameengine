#include "DevGui.h"

#include "Engine.h"
#include "core/Locator.h"

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl3.h>

namespace Engine
{
    void ShowMetricsOverlay(bool* show);

    DevGuiImpl::DevGuiImpl(const Window* window)
        : DevGui{}, m_window{window}
    {
    }

    void DevGuiImpl::Init()
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io{ImGui::GetIO()};
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.ConfigDpiScaleFonts = true;
        io.ConfigDpiScaleViewports = true;
        ImGui::StyleColorsDark();
        ImGuiStyle& style{ImGui::GetStyle()};
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }
        // TODO: scale based on window size
        style.FontScaleDpi = m_window->GetDisplayScale();
        style.ScaleAllSizes(SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay()));
        ImGui_ImplSDL3_InitForOpenGL(m_window->GetWindowHandle(), m_window->GetGLContext());
        ImGui_ImplOpenGL3_Init("#version 330 core");
        Locator::GetLogger()->Info("Dev GUI initialized");
    }

    void DevGuiImpl::ShutDown()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplSDL3_Shutdown();
        ImGui::DestroyContext();
        Locator::GetLogger()->Info("Dev GUI shut down");
    }

    void DevGuiImpl::ProcessEvent(const SDL_Event& event) { ImGui_ImplSDL3_ProcessEvent(&event); }

    bool DevGuiImpl::WantCaptureKeyboard() { return ImGui::GetIO().WantCaptureKeyboard; }

    bool DevGuiImpl::WantCaptureMouse() { return ImGui::GetIO().WantCaptureMouse; }

    void DevGuiImpl::NewFrame()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();
    }

    void DevGuiImpl::Render()
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

    void DevGuiImpl::Show()
    {
        ImGui::SetNextWindowPos(ImVec2{0.0f, 0.0f});
        const auto windowSize{m_window->GetSize()};
        ImGui::SetNextWindowSize(
            ImVec2{static_cast<float>(windowSize.width), static_cast<float>(windowSize.height)});
        ImGui::SetNextWindowBgAlpha(0.0f);
        ImGui::Begin(s_overlayId, nullptr,
                     ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoInputs |
                         ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoScrollbar);
        ImGui::End();
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
            ShowMetricsOverlay(&m_data.showMetricsOverlay);
        }
        if (m_data.showMetricsWindow) {
            ImGui::ShowMetricsWindow(&m_data.showMetricsWindow);
        }
        if (m_data.showDemoWindow) {
            ImGui::ShowDemoWindow(&m_data.showDemoWindow);
        }
    }

    void ShowMetricsOverlay(bool* show)
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
        }
        ImGui::End();
    }
} // namespace Engine
