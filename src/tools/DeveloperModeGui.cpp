#include "DeveloperModeGui.h"
#include <imgui/imgui.h>
#include <imgui/imgui_impl_sdl3.h>
#include <imgui/imgui_impl_sdlrenderer3.h>

namespace Engine
{
    DeveloperModeGui::DeveloperModeGui(const Window& window, Renderer* renderer)
        : m_renderer{renderer}
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io{ImGui::GetIO()};
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        ImGui::StyleColorsDark();
        ImGui_ImplSDL3_InitForSDLRenderer(window.getWindowPtr(), renderer->getRenderingContext());
        ImGui_ImplSDLRenderer3_Init(renderer->getRenderingContext());
        ImFontConfig config{};
        config.SizePixels = 20.0f;
        ImGui::GetIO().Fonts->AddFontDefault(&config);
        ImGui::GetStyle().ScaleAllSizes(2.0f);
    }

    DeveloperModeGui::~DeveloperModeGui()
    {
        ImGui_ImplSDLRenderer3_Shutdown();
        ImGui_ImplSDL3_Shutdown();
        ImGui::DestroyContext();
    }

    void DeveloperModeGui::processEvent(SDL_Event& event) { ImGui_ImplSDL3_ProcessEvent(&event); }

    void DeveloperModeGui::newFrame()
    {
        ImGui_ImplSDLRenderer3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();
    }

    void DeveloperModeGui::render()
    {
        ImGui::Render();
        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), m_renderer->getRenderingContext());
    }
} // namespace Engine
