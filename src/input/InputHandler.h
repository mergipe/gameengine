#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include "Command.h"
#include <SDL3/SDL.h>
#include <functional>
#include <map>
#include <memory>

namespace Engine
{
    class InputHandler
    {
    public:
        void bindKeyStateCommand(SDL_Scancode key, const std::function<void()>& command);
        void bindKeyEventCommand(SDL_Scancode key, const std::function<void()>& command, bool allowRepeat);
        void handleInputState();
        void handleKeyEvent(const SDL_KeyboardEvent& event) const;

    private:
        static constexpr int s_keyCount{SDL_SCANCODE_COUNT};
        std::map<SDL_Scancode, std::unique_ptr<Command>> m_keyStateCommands{};
        std::array<std::unique_ptr<Command>, s_keyCount> m_keyEventCommands{};
    };
} // namespace Engine

#endif
