#include "InputHandler.h"
#include <SDL3/SDL_keyboard.h>

namespace Engine
{
    void InputHandler::bindKeyStateCommand(SDL_Scancode key, const std::function<void()>& command)
    {
        if (key >= s_keyCount)
            return;
        m_keyEventCommands[key] = nullptr;
        m_keyStateCommands.insert(std::make_pair(key, command));
    }

    void InputHandler::bindKeyEventCommand(SDL_Scancode key, const std::function<void()>& command)
    {
        if (key >= s_keyCount)
            return;
        if (m_keyStateCommands.contains(key)) {
            m_keyStateCommands.erase(key);
        }
        m_keyEventCommands[key] = command;
    }

    void InputHandler::handleInputState()
    {
        int keyCount{};
        const bool* keyboardState{SDL_GetKeyboardState(&keyCount)};
        for (const auto& [key, command] : m_keyStateCommands) {
            if (keyboardState[key]) {
                command();
            }
        }
    }

    void InputHandler::handleInputEvent(SDL_Scancode key)
    {
        if (m_keyEventCommands.at(key)) {
            m_keyEventCommands[key]();
        }
    }
} // namespace Engine
