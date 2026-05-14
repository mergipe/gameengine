#ifndef CAMERA_H
#define CAMERA_H

#include "Shapes.h"
#include "core/StringId.h"

#include <glm/glm.hpp>
#include <optional>

namespace Engine
{
    enum class ProjectionType { orthographic, perspective };

    constexpr std::optional<ProjectionType> ParseProjectionType(const StringId& projectionTypeName)
    {
        using enum ProjectionType;
        if (projectionTypeName == SID("orthographic"))
            return orthographic;
        if (projectionTypeName == SID("perspective"))
            return perspective;
        return {};
    }

    class Camera
    {
    public:
        void SetViewport(float viewportWidth, float viewportHeight);
        void SetProjectionType(ProjectionType projectionType);
        void SetZNear(float zNear);
        void SetZFar(float zFar);
        void SetFovY(float fovY);
        [[nodiscard]] glm::mat4 GetProjectionTransformation() const;
        [[nodiscard]] Rect GetNearPlaneGeometry() const;
        [[nodiscard]] Rect GetFarPlaneGeometry() const;
        [[nodiscard]] glm::mat4 GetCameraTransformation() const;
        void SetModelTransformation(const glm::mat4& modelTransformation);

    private:
        glm::mat4 m_modelTransformation{};
        glm::vec3 m_position{};
        ProjectionType m_projectionType{ProjectionType::orthographic};
        float m_zNear{};
        float m_zFar{};
        float m_viewportWidth{};
        float m_viewportHeight{};
        float m_fovY{};
        float m_aspectRatio{};
    };
} // namespace Engine

#endif
