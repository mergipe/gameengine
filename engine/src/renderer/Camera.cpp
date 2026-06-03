#include "Camera.h"

#include "core/Math.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Engine
{
    AABB Camera::GetNearPlaneAABB() const
    {
        if (m_projectionType == ProjectionType::orthographic) {
            return AABB{m_position, m_orthoSize * m_aspectRatio, m_orthoSize};
        }
        const float height{glm::tan(m_perspectiveFovY / 2) * m_zNear * 2};
        return AABB{m_position, height * m_aspectRatio, height};
    }

    AABB Camera::GetFarPlaneAABB() const
    {
        if (m_projectionType == ProjectionType::orthographic) {
            return GetNearPlaneAABB();
        }
        const float height{glm::tan(m_perspectiveFovY / 2) * m_zFar * 2};
        return AABB{m_position, height * m_aspectRatio, height};
    }

    const glm::mat4& Camera::GetViewMatrix()
    {
        if (m_viewDirty) {
            UpdateViewMatrix();
        }
        return m_viewMatrix;
    }

    const glm::mat4& Camera::GetProjectionMatrix()
    {
        if (m_projectionDirty) {
            UpdateProjectionMatrix();
        }
        return m_projectionMatrix;
    }

    glm::vec3 Camera::GetNormalizedScreenPosition(glm::vec3 worldPosition) const
    {
        const auto ndcPosition{m_projectionMatrix * m_viewMatrix * glm::vec4{worldPosition, 1.0f}};
        return (glm::vec3{ndcPosition.x, -ndcPosition.y, ndcPosition.z} + glm::vec3{1.0f, 1.0f, 0.0f}) * 0.5f;
    }

    void Camera::SetViewport(float viewportWidth, float viewportHeight)
    {
        m_aspectRatio = viewportWidth / viewportHeight;
        m_projectionDirty = true;
    }

    void Camera::SetProjectionType(ProjectionType projectionType)
    {
        m_projectionType = projectionType;
        m_projectionDirty = true;
    }

    void Camera::SetZNear(float zNear)
    {
        m_zNear = zNear;
        m_projectionDirty = true;
    }

    void Camera::SetZFar(float zFar)
    {
        m_zFar = zFar;
        m_projectionDirty = true;
    }

    void Camera::SetPerspectiveFovY(float perspectiveFovY)
    {
        m_perspectiveFovY = perspectiveFovY;
        m_projectionDirty = true;
    }

    void Camera::SetOrthoSize(float orthoSize)
    {
        m_orthoSize = orthoSize;
        m_projectionDirty = true;
    }

    void Camera::SetTransform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
    {
        m_transformMatrix = Math::GetTransformMatrix(position, rotation, scale);
        m_position = position;
        m_viewDirty = true;
        m_projectionDirty = true;
    }

    void Camera::UpdateViewMatrix()
    {
        const glm::vec3 gaze{-(m_transformMatrix * glm::vec4{0.0f, 0.0f, 1.0f, 0.0f})};
        const glm::vec3 up{m_transformMatrix * glm::vec4{0.0f, 1.0f, 0.0f, 0.0f}};
        m_viewMatrix = glm::lookAt(m_position, m_position + gaze, up);
        m_viewDirty = false;
    }

    void Camera::UpdateProjectionMatrix()
    {
        if (m_projectionType == ProjectionType::orthographic) {
            const AABB nearPlaneAABB{GetNearPlaneAABB()};
            m_projectionMatrix =
                glm::ortho(nearPlaneAABB.GetLeftX(), nearPlaneAABB.GetRightX(), nearPlaneAABB.GetBottomY(),
                           nearPlaneAABB.GetTopY(), m_zNear, m_zFar);
        } else {
            m_projectionMatrix = glm::perspective(m_perspectiveFovY, m_aspectRatio, m_zNear, m_zFar);
        }
        m_projectionDirty = false;
    }
} // namespace Engine
