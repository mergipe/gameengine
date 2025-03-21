#include "Input.h"
#include <SDL3/SDL_keyboard.h>

namespace Engine
{
    void InputHandler::handleInput()
    {
        int keyCount{};
        const bool* keyboardState{SDL_GetKeyboardState(&keyCount)};
        for (const auto& [key, command] : m_commands) {
            if (keyboardState[key]) {
                command->execute();
            }
        }
    }

    void InputHandler::bindCommand(SDL_Scancode key, const Command& command)
    {
        if (key >= s_keyCount)
            return;
        m_commands.emplace(key, std::make_unique<Command>(command));
    }
} // namespace Engine
