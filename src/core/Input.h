#ifndef INPUT_H
#define INPUT_H

#include <SDL3/SDL_scancode.h>
#include <map>
#include <memory>

namespace Engine
{
    class Command
    {
    public:
        void execute();
    };

    class InputHandler
    {
    public:
        void handleInput();
        void bindCommand(SDL_Scancode key, const Command& command);

    private:
        static constexpr int s_keyCount{SDL_SCANCODE_COUNT};
        std::map<SDL_Scancode, std::unique_ptr<Command>> m_commands{};
    };
} // namespace Engine

#endif
