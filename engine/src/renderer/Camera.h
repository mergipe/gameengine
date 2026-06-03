#ifndef CAMERA_H
#define CAMERA_H

#include "core/AABB.h"
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
        [[nodiscard]] AABB GetNearPlaneAABB() const;
        [[nodiscard]] AABB GetFarPlaneAABB() const;
        [[nodiscard]] const glm::mat4& GetViewMatrix();
        [[nodiscard]] const glm::mat4& GetProjectionMatrix();
        [[nodiscard]] glm::vec3 GetNormalizedScreenPosition(glm::vec3 worldPosition) const;
        void SetViewport(float viewportWidth, float viewportHeight);
        void SetProjectionType(ProjectionType projectionType);
        void SetZNear(float zNear);
        void SetZFar(float zFar);
        void SetPerspectiveFovY(float perspectiveFovY);
        void SetOrthoSize(float orthoSize);
        void SetTransform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);

    private:
        void UpdateViewMatrix();
        void UpdateProjectionMatrix();

        glm::mat4 m_transformMatrix{};
        glm::mat4 m_viewMatrix{};
        glm::mat4 m_projectionMatrix{};
        glm::vec3 m_position{};
        ProjectionType m_projectionType{ProjectionType::orthographic};
        float m_zNear{};
        float m_zFar{};
        float m_aspectRatio{};
        float m_orthoSize{};
        float m_perspectiveFovY{};
        bool m_viewDirty{true};
        bool m_projectionDirty{true};
    };
} // namespace Engine

#endif
