#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include <SDL3/SDL_scancode.h>
#include <functional>
#include <map>

namespace Engine
{
    class InputHandler
    {
    public:
        void bindKeyStateCommand(SDL_Scancode key, const std::function<void()>& command);
        void bindKeyEventCommand(SDL_Scancode key, const std::function<void()>& command);
        void handleInputState();
        void handleInputEvent(SDL_Scancode key);

    private:
        static constexpr int s_keyCount{SDL_SCANCODE_COUNT};
        std::map<SDL_Scancode, std::function<void()>> m_keyStateCommands{};
        std::array<std::function<void()>, s_keyCount> m_keyEventCommands{};
    };
} // namespace Engine

#endif
