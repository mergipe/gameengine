#include "DebugRenderer.h"

#include "RenderManager.h"
#include "core/Assert.h"
#include "debug/DevGui.h"

#include <imgui.h>

namespace Engine
{
    DebugRenderer::DebugRenderer(RenderContext* renderContext)
        : m_renderContext{renderContext}
    {
    }

    VertexBuffer CreateQuadVertexBuffer()
    {
        constexpr static std::array quadVertices{glm::vec2{1.1f, 1.1f}, glm::vec2{1.1f, -1.1f},
                                                 glm::vec2{-1.1f, -1.1f}, glm::vec2{-1.1f, 1.1f}};
        VertexBuffer quadVertexBuffer{};
        quadVertexBuffer.Create(sizeof(quadVertices), quadVertices.data());
        quadVertexBuffer.AddAttribute(VertexAttribute{0, GL_FLOAT, 2, false});
        return quadVertexBuffer;
    }

    ElementBuffer CreateQuadElementBuffer()
    {
        constexpr static std::array quadVertexIndices{0, 1, 3, 1, 2, 3};
        ElementBuffer elementBuffer{};
        elementBuffer.Create(sizeof(quadVertexIndices), quadVertexIndices.data());
        return elementBuffer;
    }

    void DebugRenderer::Init()
    {
        m_pointVertexArray.Create();
        VertexBuffer pointVertexBuffer{};
        pointVertexBuffer.Create(s_pointsBatchSize * sizeof(PointData));
        pointVertexBuffer.AddAttribute(VertexAttribute{0, GL_FLOAT, 2, false});
        pointVertexBuffer.AddAttribute(VertexAttribute{1, GL_FLOAT, 1, false});
        pointVertexBuffer.AddAttribute(VertexAttribute{2, GL_UNSIGNED_BYTE, 4, true});
        m_pointVertexArray.AddVertexBuffer(std::move(pointVertexBuffer));
        m_pointVertexArray.ConfigureVertexAttributes();
        m_renderContext->shaderManager->LoadShader(s_pointShaderId, "point.vert", "point.frag")
            .Use()
            .BindUniformBlock("Camera", m_renderContext->cameraUniformBuffer.GetBindingPoint());

        m_lineVertexArray.Create();
        VertexBuffer lineVertexBuffer{};
        lineVertexBuffer.Create(s_linesBatchSize * sizeof(VertexData));
        lineVertexBuffer.AddAttribute(VertexAttribute{0, GL_FLOAT, 2, false});
        lineVertexBuffer.AddAttribute(VertexAttribute{1, GL_UNSIGNED_BYTE, 4, true});
        m_lineVertexArray.AddVertexBuffer(std::move(lineVertexBuffer));
        m_lineVertexArray.ConfigureVertexAttributes();
        m_renderContext->shaderManager->LoadShader(s_lineShaderId, "line.vert", "line.frag")
            .Use()
            .BindUniformBlock("Camera", m_renderContext->cameraUniformBuffer.GetBindingPoint());

        m_circleVertexArray.Create();
        m_circleVertexArray.AddVertexBuffer(CreateQuadVertexBuffer());
        VertexBuffer circleVertexBuffer{};
        circleVertexBuffer.Create(s_circlesBatchSize * sizeof(CircleData), true);
        circleVertexBuffer.AddAttribute(VertexAttribute{1, GL_FLOAT, 2, false});
        circleVertexBuffer.AddAttribute(VertexAttribute{2, GL_FLOAT, 1, false});
        circleVertexBuffer.AddAttribute(VertexAttribute{3, GL_UNSIGNED_BYTE, 4, true});
        m_circleVertexArray.AddVertexBuffer(std::move(circleVertexBuffer));
        m_circleVertexArray.ConfigureVertexAttributes();
        m_circleVertexArray.SetElementBuffer(CreateQuadElementBuffer());
        m_renderContext->shaderManager->LoadShader(s_circleShaderId, "circle.vert", "circle.frag")
            .Use()
            .BindUniformBlock("Camera", m_renderContext->cameraUniformBuffer.GetBindingPoint());

        m_polygonVertexArray.Create();
        m_polygonVertexArray.AddVertexBuffer(CreateQuadVertexBuffer());
        VertexBuffer polygonVertexBuffer{};
        polygonVertexBuffer.Create(s_polygonsBatchSize * sizeof(PolygonData), true);
        polygonVertexBuffer.AddAttribute(VertexAttribute{1, GL_FLOAT, 4, false});
        polygonVertexBuffer.AddAttribute(VertexAttribute{2, GL_FLOAT, 4, false});
        polygonVertexBuffer.AddAttribute(VertexAttribute{3, GL_FLOAT, 4, false});
        polygonVertexBuffer.AddAttribute(VertexAttribute{4, GL_FLOAT, 4, false});
        polygonVertexBuffer.AddAttribute(VertexAttribute{5, GL_FLOAT, 4, false});
        polygonVertexBuffer.AddAttribute(VertexAttribute{6, GL_INT, 1, false});
        polygonVertexBuffer.AddAttribute(VertexAttribute{7, GL_FLOAT, 1, false});
        polygonVertexBuffer.AddAttribute(VertexAttribute{8, GL_UNSIGNED_BYTE, 4, true});
        m_polygonVertexArray.AddVertexBuffer(std::move(polygonVertexBuffer));
        m_polygonVertexArray.ConfigureVertexAttributes();
        m_polygonVertexArray.SetElementBuffer(CreateQuadElementBuffer());
        m_renderContext->shaderManager->LoadShader(s_polygonShaderId, "polygon.vert", "polygon.frag")
            .Use()
            .BindUniformBlock("Camera", m_renderContext->cameraUniformBuffer.GetBindingPoint());
    }

    void DebugRenderer::ShutDown()
    {
        m_pointVertexArray.Destroy();
        m_lineVertexArray.Destroy();
        m_circleVertexArray.Destroy();
    }

    void DebugRenderer::AddPoint(glm::vec2 position, float size, RGBA8 color)
    {
        m_points.emplace_back(position, size, color);
    }

    void DebugRenderer::AddLine(glm::vec2 point1, glm::vec2 point2, RGBA8 color)
    {
        m_linePoints.emplace_back(point1, color);
        m_linePoints.emplace_back(point2, color);
    }

    void DebugRenderer::AddCircle(glm::vec2 center, float radius, RGBA8 color)
    {
        m_circles.emplace_back(center, radius, color);
    }

    void DebugRenderer::AddPolygon(const glm::vec2* vertices, int vertexCount, float radius,
                                   glm::vec4 transform, RGBA8 color)
    {
        PolygonData polygon{};
        polygon.transform = transform;
        glm::vec2* vertexPtr{&polygon.vertex1};
        for (std::size_t i{0}; i < static_cast<std::size_t>(std::min(vertexCount, 8)); ++i) {
            vertexPtr[i] = vertices[i];
        }
        polygon.vertexCount = vertexCount;
        polygon.radius = radius;
        polygon.color = color;
        m_polygons.push_back(polygon);
    }

    void DebugRenderer::AddTransformAxes(glm::vec2 position, float rotation, float size)
    {
        if (!m_debugUIData.drawTransformAxes) {
            return;
        }
        const float cos{glm::cos(rotation)};
        const float sin{glm::sin(rotation)};
        AddLine(position, position + size * glm::vec2{cos, sin}, RGBA8{255, 0, 0, 255});
        AddLine(position, position + size * glm::vec2{-sin, cos}, RGBA8{0, 255, 0, 255});
    }

    void DebugRenderer::AddText(glm::vec2 position, std::string_view text, RGBA8 color)
    {
        m_texts.emplace_back(std::string{text}, position, color);
    }

    void DebugRenderer::Flush()
    {
        FlushPolygons();
        FlushCircles();
        FlushLines();
        FlushPoints();
        FlushTexts();
    }

    void DebugRenderer::RenderDevGui()
    {
        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("Rendering")) {
                ImGui::Checkbox("Draw transform axes", &m_debugUIData.drawTransformAxes);
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }
    }

    void DebugRenderer::FlushPoints()
    {
        int count{static_cast<int>(m_points.size())};
        if (count == 0) {
            return;
        }
        m_renderContext->shaderManager->GetShader(s_pointShaderId).Use();
        m_pointVertexArray.Bind();
        auto* buffer{m_pointVertexArray.GetVertexBuffer(0)};
        glEnable(GL_PROGRAM_POINT_SIZE);
        std::size_t offset{0};
        while (count > 0) {
            const int batchSize{std::min(count, s_pointsBatchSize)};
            buffer->UpdateData(0, static_cast<U32>(batchSize) * sizeof(PointData), &m_points[offset]);
            glDrawArrays(GL_POINTS, 0, batchSize);
            count -= s_pointsBatchSize;
            offset += s_pointsBatchSize;
        }
        glDisable(GL_PROGRAM_POINT_SIZE);
        m_pointVertexArray.Unbind();
        m_points.clear();
    }

    void DebugRenderer::FlushLines()
    {
        int count{static_cast<int>(m_linePoints.size())};
        if (count == 0) {
            return;
        }
        ASSERT(count % 2 == 0);
        m_renderContext->shaderManager->GetShader(s_lineShaderId).Use();
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        m_lineVertexArray.Bind();
        auto* buffer{m_lineVertexArray.GetVertexBuffer(0)};
        std::size_t offset{0};
        while (count > 0) {
            const int batchSize{std::min(count, s_linesBatchSize)};
            buffer->UpdateData(0, static_cast<U32>(batchSize) * sizeof(VertexData), &m_linePoints[offset]);
            glDrawArrays(GL_LINES, 0, batchSize);
            count -= s_linesBatchSize;
            offset += s_linesBatchSize;
        }
        m_lineVertexArray.Unbind();
        glDisable(GL_BLEND);
        m_linePoints.clear();
    }

    void DebugRenderer::FlushCircles()
    {
        int count{static_cast<int>(m_circles.size())};
        if (count == 0) {
            return;
        }
        const auto* elementBuffer{m_circleVertexArray.GetElementBuffer()};
        m_renderContext->shaderManager->GetShader(s_circleShaderId)
            .Use()
            .SetUniform("u_pixelScale", static_cast<float>(m_renderContext->viewportHeight));
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        m_circleVertexArray.Bind();
        auto* buffer{m_circleVertexArray.GetVertexBuffer(1)};
        std::size_t offset{0};
        while (count > 0) {
            const int batchSize{std::min(count, s_circlesBatchSize)};
            buffer->UpdateData(0, static_cast<U32>(batchSize) * sizeof(CircleData), &m_circles[offset]);
            glDrawElementsInstanced(GL_TRIANGLES, static_cast<GLsizei>(elementBuffer->GetSize()),
                                    GL_UNSIGNED_INT, nullptr, batchSize);
            count -= s_circlesBatchSize;
            offset += s_circlesBatchSize;
        }
        m_circleVertexArray.Unbind();
        glDisable(GL_BLEND);
        m_circles.clear();
    }

    void DebugRenderer::FlushPolygons()
    {
        int count{static_cast<int>(m_polygons.size())};
        if (count == 0) {
            return;
        }
        const auto* elementBuffer{m_polygonVertexArray.GetElementBuffer()};
        m_renderContext->shaderManager->GetShader(s_polygonShaderId)
            .Use()
            .SetUniform("u_pixelScale", static_cast<float>(m_renderContext->viewportHeight));
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        m_polygonVertexArray.Bind();
        auto* buffer{m_polygonVertexArray.GetVertexBuffer(1)};
        std::size_t offset{0};
        while (count > 0) {
            const int batchSize{std::min(count, s_polygonsBatchSize)};
            buffer->UpdateData(0, static_cast<U32>(batchSize) * sizeof(PolygonData), &m_polygons[offset]);
            glDrawElementsInstanced(GL_TRIANGLES, static_cast<GLsizei>(elementBuffer->GetSize()),
                                    GL_UNSIGNED_INT, nullptr, batchSize);
            count -= s_polygonsBatchSize;
            offset += s_polygonsBatchSize;
        }
        m_polygonVertexArray.Unbind();
        glDisable(GL_BLEND);
        m_polygons.clear();
    }

    void DebugRenderer::FlushTexts()
    {
        if (m_renderContext->camera) {
            for (const auto& textData : m_texts) {
                if (ImGui::Begin(DevGui::s_overlayId)) {
                    const auto viewportSize{ImGui::GetWindowViewport()->Size};
                    const glm::vec2 screenPosition{
                        glm::vec2{m_renderContext->camera->GetNormalizedScreenPosition(
                            glm::vec3{textData.position, 0.0f})} *
                        glm::vec2{viewportSize.x, viewportSize.y}};
                    ImGui::SetCursorPos(ImVec2{screenPosition.x, screenPosition.y});
                    ImGui::TextColored(
                        ImColor{textData.color.r, textData.color.g, textData.color.b, textData.color.a},
                        textData.text.data());
                }
                ImGui::End();
            }
        }
        m_texts.clear();
    }
} // namespace Engine
