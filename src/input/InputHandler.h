#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

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
        const StringId& GetCurrentScopeId() const;
        const StringId& GetPreviousScopeId() const;
        const StringId& GetDevGuiScopeId() const { return m_devGuiScopeId; }
        void SwitchScope(const StringId& scopeId);
        void HandleKeyboardKeyDownEvent(const SDL_KeyboardEvent& event);
        void HandleKeyboardKeyUpEvent(const SDL_KeyboardEvent& event);
        void ResolveInput();
        const InputDevice::Id& AcquireAvailableDevice();

    private:
        InputCommand* FindCommand(InputDevice::Type deviceType, int controlCode);
        void TriggerCommand(const InputCommand& command, const InputDevice::Id& deviceId,
                            const InputValue& inputValue);
        void HandleControlDown(const InputCommand& command, const InputDevice::Id& deviceId);
        void HandleControlUp(const InputCommand& command, const InputDevice::Id& deviceId);
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
