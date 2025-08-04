#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include "InputCallback.h"
#include "InputCommand.h"
#include "InputConfig.h"
#include "core/StringId.h"
#include "events/Events.h"
#include <SDL3/SDL.h>
#include <array>
#include <filesystem>
#include <vector>

namespace Engine
{
    class InputHandler
    {
    public:
        explicit InputHandler(const std::filesystem::path& inputConfigFilepath);
        InputHandler(const InputHandler&) = delete;
        InputHandler(InputHandler&&) = delete;
        InputHandler& operator=(const InputHandler&) = delete;
        InputHandler& operator=(InputHandler&&) = delete;
        ~InputHandler() = default;
        const StringId& getCurrentScopeId() const;
        const StringId& getPreviousScopeId() const;
        const StringId& getDevGuiScopeId() const { return m_devGuiScopeId; }
        void switchScope(const StringId& scopeId);
        void handleKeyboardKeyDownEvent(const SDL_KeyboardEvent& event);
        void handleKeyboardKeyUpEvent(const SDL_KeyboardEvent& event);
        void resolveInput();
        const InputDevice::Id& acquireAvailableDevice();

    private:
        InputCommand* findCommand(InputDevice::Type deviceType, int controlCode);
        void triggerCommand(const InputCommand& command, const InputDevice::Id& deviceId,
                            const InputValue& inputValue);
        void handleControlDown(const InputCommand& command, const InputDevice::Id& deviceId);
        void handleControlUp(const InputCommand& command, const InputDevice::Id& deviceId);
        static constexpr int s_maxInputEventsByFrame{16};
        std::array<InputEvent, s_maxInputEventsByFrame> m_unhandledInputEvents{};
        InputConfig m_inputConfig{};
        InputCallbackMapping m_engineCallbackMapping{};
        std::vector<InputDevice> m_inputDevices{};
        StringId m_devGuiScopeId{"dev-gui"};
        InputScope* m_currentScope{};
        InputScope* m_previousScope{};
        std::size_t m_inputEventsCount{};
    };
} // namespace Engine

#endif
