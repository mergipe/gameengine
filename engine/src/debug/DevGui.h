#ifndef DEV_GUI_H
#define DEV_GUI_H

#include "core/Window.h"

namespace Engine
{
    class DevGui
    {
    public:
        virtual ~DevGui() = default;
        virtual void ProcessEvent(const SDL_Event& event) = 0;
        virtual bool WantCaptureKeyboard() = 0;
        virtual bool WantCaptureMouse() = 0;
        virtual void NewFrame() = 0;
        virtual void Render() = 0;
        virtual void Show() = 0;
    };

    class NullDevGui final : public DevGui
    {
    public:
        void ProcessEvent([[maybe_unused]] const SDL_Event& event) override {}
        bool WantCaptureKeyboard() override { return false; }
        bool WantCaptureMouse() override { return false; }
        void NewFrame() override {}
        void Render() override {}
        void Show() override {}
    };

    struct DevGuiData {
        bool showMetricsOverlay{true};
        bool showMetricsWindow{false};
        bool showDemoWindow{false};
    };

    class DevGuiImpl final : public DevGui
    {
    public:
        explicit DevGuiImpl(const Window& window);
        DevGuiImpl(const DevGuiImpl&) = delete;
        DevGuiImpl(DevGuiImpl&&) = delete;
        DevGuiImpl& operator=(const DevGuiImpl&) = delete;
        DevGuiImpl& operator=(DevGuiImpl&&) = delete;
        ~DevGuiImpl() override;
        void ProcessEvent(const SDL_Event& event) override;
        bool WantCaptureKeyboard() override;
        bool WantCaptureMouse() override;
        void NewFrame() override;
        void Render() override;
        void Show() override;

    private:
        DevGuiData m_data{};
    };
} // namespace Engine

#endif
