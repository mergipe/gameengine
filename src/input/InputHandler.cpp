#include "InputHandler.h"
#include "core/Assert.h"
#include <SDL3/SDL_keyboard.h>

namespace Engine
{
    void InputHandler::bindKeyStateCommand(SDL_Scancode key, const std::function<void()>& command)
    {
        ASSERT(key >= 0 && key < s_keyCount);
        m_keyEventCommands[key] = nullptr;
        m_keyStateCommands.emplace(key, std::make_unique<Command>(command, true));
    }

    void InputHandler::bindKeyEventCommand(SDL_Scancode key, const std::function<void()>& command,
                                           bool allowRepeat)
    {
        ASSERT(key >= 0 && key < s_keyCount);
        if (m_keyStateCommands.contains(key)) {
            m_keyStateCommands.erase(key);
        }
        m_keyEventCommands[key] = std::make_unique<Command>(command, allowRepeat);
    }

    void InputHandler::handleInputState()
    {
        int keyCount{};
        const bool* keyboardState{SDL_GetKeyboardState(&keyCount)};
        for (const auto& [key, command] : m_keyStateCommands) {
            if (keyboardState[key]) {
                command->execute();
            }
        }
    }

    void InputHandler::handleKeyEvent(const SDL_KeyboardEvent& event) const
    {
        if (m_keyEventCommands.at(event.scancode)) {
            const auto& command{*m_keyEventCommands[event.scancode].get()};
            if (!command.allowRepeat() && event.repeat) {
                return;
            }
            command.execute();
        }
    }
} // namespace Engine
