#ifndef DEBUG_RENDERER_H
#define DEBUG_RENDERER_H

#include "RenderContext.h"
#include "core/StringId.h"
#include "core/Types.h"

#include <glm/glm.hpp>

namespace Engine
{
    struct DebugRendererUIData {
        bool drawTransformAxes{true};
    };

    struct PointData {
        glm::vec2 position{};
        float size{};
        RGBA8 color{};
    };

    struct VertexData {
        glm::vec2 position{};
        RGBA8 color{};
    };

    struct CircleData {
        glm::vec2 position{};
        float radius{};
        RGBA8 color{};
    };

    struct PolygonData {
        glm::vec4 transform{};
        glm::vec2 vertex1{};
        glm::vec2 vertex2{};
        glm::vec2 vertex3{};
        glm::vec2 vertex4{};
        glm::vec2 vertex5{};
        glm::vec2 vertex6{};
        glm::vec2 vertex7{};
        glm::vec2 vertex8{};
        int vertexCount{};
        float radius{};
        RGBA8 color{};
    };

    struct TextData {
        std::string text{};
        glm::vec2 position{};
        RGBA8 color{};
    };

    class DebugRenderer
    {
    public:
        explicit DebugRenderer(RenderContext* renderContext);
        void Init();
        void ShutDown();
        void AddPoint(glm::vec2 position, float size, RGBA8 color);
        void AddLine(glm::vec2 point1, glm::vec2 point2, RGBA8 color);
        void AddCircle(glm::vec2 center, float radius, RGBA8 color);
        void AddPolygon(const glm::vec2* vertices, int vertexCount, float radius, glm::vec4 transform,
                        RGBA8 color);
        void AddTransformAxes(glm::vec2 position, float rotation, float size);
        void AddText(glm::vec2 position, std::string_view text, RGBA8 color);
        void Flush();
        void RenderDevGui();

    private:
        void FlushPoints();
        void FlushLines();
        void FlushCircles();
        void FlushPolygons();
        void FlushTexts();

        constexpr static StringId s_pointShaderId{SID("point-shader")};
        constexpr static StringId s_lineShaderId{SID("line-shader")};
        constexpr static StringId s_circleShaderId{SID("circle-shader")};
        constexpr static StringId s_polygonShaderId{SID("polygon-shader")};
        constexpr static int s_pointsBatchSize{2048};
        constexpr static int s_linesBatchSize{2 * 2048};
        constexpr static int s_circlesBatchSize{2048};
        constexpr static int s_polygonsBatchSize{512};

        VertexArray m_pointVertexArray{};
        VertexArray m_lineVertexArray{};
        VertexArray m_circleVertexArray{};
        VertexArray m_polygonVertexArray{};

        std::vector<PointData> m_points{};
        std::vector<VertexData> m_linePoints{};
        std::vector<CircleData> m_circles{};
        std::vector<PolygonData> m_polygons{};
        std::vector<TextData> m_texts{};

        RenderContext* m_renderContext{};
        DebugRendererUIData m_debugUIData{};
    };
} // namespace Engine

#endif
