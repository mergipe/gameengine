#ifndef SCRIPT_H
#define SCRIPT_H

#include <sol/sol.hpp>

namespace Engine
{
    class Script
    {
    public:
        explicit Script(const sol::table& instance);
        void onStart();
        void onUpdate(float timeStep);

    private:
        sol::table m_scriptInstance{};
    };
} // namespace Engine

#endif
