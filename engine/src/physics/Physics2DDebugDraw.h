#ifndef PHYSICS_2D_DEBUG_DRAW_H
#define PHYSICS_2D_DEBUG_DRAW_H

#include <box2d/box2d.h>

namespace Engine
{
    class Physics2DDebugDraw
    {
    public:
        Physics2DDebugDraw();
        void Draw(b2WorldId worldId);
        void RenderDevGui();

    private:
        b2DebugDraw m_debugDraw{};
    };
} // namespace Engine

#endif
