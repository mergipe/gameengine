#include "Physics2DDebugDraw.h"

#include "core/Locator.h"

#include <imgui.h>

namespace Engine
{
    static std::array<glm::vec2, B2_MAX_POLYGON_VERTICES> polygonVertices{};

    void DrawPolygon(const b2Vec2* vertices, int vertexCount, b2HexColor color, void* context)
    {
        auto p1{vertices[vertexCount - 1]};
        for (int i{0}; i < vertexCount; ++i) {
            const auto p2{vertices[i]};
            static_cast<DebugRenderer*>(context)->AddLine(glm::vec2{p1.x, p1.y}, glm::vec2{p2.x, p2.y},
                                                          RGBA8::FromHex(color));
            p1 = p2;
        }
    }

    void DrawSolidPolygon(b2Transform transform, const b2Vec2* vertices, int vertexCount, float radius,
                          b2HexColor color, void* context)
    {
        for (std::size_t i{0}; i < static_cast<std::size_t>(vertexCount); ++i) {
            polygonVertices[i] = glm::vec2{vertices[i].x, vertices[i].y};
        }
        static_cast<DebugRenderer*>(context)->AddPolygon(
            polygonVertices.data(), vertexCount, radius,
            glm::vec4{transform.p.x, transform.p.y, transform.q.c, transform.q.s}, RGBA8::FromHex(color));
    }

    void DrawCircle(b2Vec2 center, float radius, b2HexColor color, void* context)
    {
        static_cast<DebugRenderer*>(context)->AddCircle(glm::vec2{center.x, center.y}, radius,
                                                        RGBA8::FromHex(color));
    }

    void DrawSolidCircle(b2Transform transform, float radius, b2HexColor color, void* context)
    {
        DrawCircle(transform.p, radius, color, context);
    }

    void DrawSolidCapsule(b2Vec2 p1, b2Vec2 p2, float radius, b2HexColor color, void* context)
    {
        // TODO
        (void) p1;
        (void) p2;
        (void) radius;
        (void) color;
        (void) context;
    }

    void DrawSegment(b2Vec2 p1, b2Vec2 p2, b2HexColor color, void* context)
    {
        static_cast<DebugRenderer*>(context)->AddLine(glm::vec2{p1.x, p1.y}, glm::vec2{p2.x, p2.y},
                                                      RGBA8::FromHex(color));
    }

    void DrawTransform([[maybe_unused]] b2Transform transform, [[maybe_unused]] void* context) {}

    void DrawPoint(b2Vec2 p, float size, b2HexColor color, void* context)
    {
        static_cast<DebugRenderer*>(context)->AddPoint(glm::vec2{p.x, p.y}, size, RGBA8::FromHex(color));
    }

    void DrawString(b2Vec2 p, const char* s, b2HexColor color, void* context)
    {
        static_cast<DebugRenderer*>(context)->AddText(glm::vec2{p.x, p.y}, s, RGBA8::FromHex(color));
    }

    Physics2DDebugDraw::Physics2DDebugDraw()
    {
        m_debugDraw.DrawPolygonFcn = DrawPolygon;
        m_debugDraw.DrawSolidPolygonFcn = DrawSolidPolygon;
        m_debugDraw.DrawCircleFcn = DrawCircle;
        m_debugDraw.DrawSolidCircleFcn = DrawSolidCircle;
        m_debugDraw.DrawSolidCapsuleFcn = DrawSolidCapsule;
        m_debugDraw.DrawSegmentFcn = DrawSegment;
        m_debugDraw.DrawTransformFcn = DrawTransform;
        m_debugDraw.DrawPointFcn = DrawPoint;
        m_debugDraw.DrawStringFcn = DrawString;
        m_debugDraw.context = Locator::GetDebugRenderer();
        m_debugDraw.drawBodyNames = false;
        m_debugDraw.drawBounds = false;
        m_debugDraw.drawContacts = false;
        m_debugDraw.drawContactNormals = false;
        m_debugDraw.drawContactImpulses = false;
        m_debugDraw.drawContactFeatures = false;
        m_debugDraw.drawFrictionImpulses = false;
        m_debugDraw.drawGraphColors = false;
        m_debugDraw.drawIslands = false;
        m_debugDraw.drawJoints = false;
        m_debugDraw.drawJointExtras = false;
        m_debugDraw.drawMass = false;
        m_debugDraw.drawShapes = true;
        m_debugDraw.useDrawingBounds = false;
    }

    void Physics2DDebugDraw::Draw(b2WorldId worldId) { b2World_Draw(worldId, &m_debugDraw); }

    void Physics2DDebugDraw::RenderDevGui()
    {
        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("Rendering")) {
                if (ImGui::BeginMenu("Physics")) {
                    ImGui::Checkbox("Draw colliders/triggers", &m_debugDraw.drawShapes);
                    ImGui::Checkbox("Draw joints", &m_debugDraw.drawJoints);
                    ImGui::Checkbox("Draw bounding boxes", &m_debugDraw.drawBounds);
                    ImGui::Checkbox("Draw contact points", &m_debugDraw.drawContacts);
                    ImGui::Checkbox("Draw contacts normals", &m_debugDraw.drawContactNormals);
                    ImGui::Checkbox("Draw contact impulses", &m_debugDraw.drawContactImpulses);
                    ImGui::Checkbox("Draw friction impulses", &m_debugDraw.drawFrictionImpulses);
                    ImGui::Checkbox("Draw mass", &m_debugDraw.drawMass);
                    ImGui::Checkbox("Draw islands", &m_debugDraw.drawIslands);
                    ImGui::EndMenu();
                }
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }
    }
} // namespace Engine
