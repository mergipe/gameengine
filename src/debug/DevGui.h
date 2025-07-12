#ifndef DEVGUI_H
#define DEVGUI_H

#include "core/Window.h"

namespace Engine
{
    class DevGui
    {
    public:
        virtual ~DevGui() = default;
        virtual void processEvent(const SDL_Event& event) = 0;
        virtual bool wantCaptureKeyboard() = 0;
        virtual bool wantCaptureMouse() = 0;
        virtual void newFrame() = 0;
        virtual void render() = 0;
        virtual void show() = 0;
    };

    class NullDevGui final : public DevGui
    {
    public:
        void processEvent([[maybe_unused]] const SDL_Event& event) override {}
        bool wantCaptureKeyboard() override { return false; }
        bool wantCaptureMouse() override { return false; }
        void newFrame() override {}
        void render() override {}
        void show() override {}
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
        void processEvent(const SDL_Event& event) override;
        bool wantCaptureKeyboard() override;
        bool wantCaptureMouse() override;
        void newFrame() override;
        void render() override;
        void show() override;

    private:
        DevGuiData m_data{};
    };
} // namespace Engine

#endif
