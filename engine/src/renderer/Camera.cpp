#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Engine
{
    void Camera::SetViewport(float viewportWidth, float viewportHeight)
    {
        m_viewportWidth = viewportWidth;
        m_viewportHeight = viewportHeight;
    }

    void Camera::SetProjectionType(ProjectionType projectionType) { m_projectionType = projectionType; }

    void Camera::SetZNear(float zNear) { m_zNear = zNear; }

    void Camera::SetZFar(float zFar) { m_zFar = zFar; }

    void Camera::SetFovY(float fovY) { m_fovY = fovY; }

    glm::mat4 Camera::GetProjectionTransformation() const // FIXME: calculated every time
    {
        if (m_projectionType == ProjectionType::orthographic) {
            const Rect nearPlaneGeometry{GetNearPlaneGeometry()};
            return glm::ortho(nearPlaneGeometry.GetLeftX(), nearPlaneGeometry.GetRightX(),
                              nearPlaneGeometry.GetBottomY(), nearPlaneGeometry.GetTopY(), m_zNear, m_zFar);
        }
        return glm::perspective(m_fovY, m_aspectRatio, m_zNear, m_zFar);
    }

    Rect Camera::GetNearPlaneGeometry() const
    {
        if (m_projectionType == ProjectionType::orthographic) {
            return Rect{m_position, m_viewportWidth, m_viewportHeight};
        }
        const float height{glm::tan(m_fovY / 2) * m_zNear * 2};
        return Rect{m_position, height * m_aspectRatio, height};
    }

    Rect Camera::GetFarPlaneGeometry() const
    {
        if (m_projectionType == ProjectionType::orthographic) {
            return GetNearPlaneGeometry();
        }
        const float height{glm::tan(m_fovY / 2) * m_zFar * 2};
        return Rect{m_position, height * m_aspectRatio, height};
    }

    glm::mat4 Camera::GetCameraTransformation() const
    {
        const glm::vec3 position{m_modelTransformation[3]};
        const glm::vec3 gaze{-(m_modelTransformation * glm::vec4{0.0f, 0.0f, 1.0f, 0.0f})};
        const glm::vec3 up{m_modelTransformation * glm::vec4{0.0f, 1.0f, 0.0f, 0.0f}};
        return glm::lookAt(position, position + gaze, up);
    }

    void Camera::SetModelTransformation(const glm::mat4& modelTransformation)
    {
        m_modelTransformation = modelTransformation;
        m_position = modelTransformation[3];
    }
} // namespace Engine
