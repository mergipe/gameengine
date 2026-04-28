#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Engine
{
    Camera::Camera(float zNear, float zFar, float viewportWidth, float viewportHeight)
        : m_zNear{zNear}
        , m_zFar{zFar}
        , m_viewportWidth{viewportWidth}
        , m_viewportHeight{viewportHeight}
        , m_aspectRatio{viewportWidth / viewportHeight}
    {
    }

    glm::mat4 Camera::GetCameraTransformation() const
    {
        const glm::vec3 position{GetModelTransformation()[3]};
        const glm::vec3 gaze{-(GetModelTransformation() * glm::vec4{0.0f, 0.0f, 1.0f, 0.0f})};
        const glm::vec3 up{GetModelTransformation() * glm::vec4{0.0f, 1.0f, 0.0f, 0.0f}};
        return glm::lookAt(position, position + gaze, up);
    }

    void Camera::SetModelTransformation(const glm::mat4& modelTransformation)
    {
        m_modelTransformation = modelTransformation;
        m_position = modelTransformation[3];
    }

    OrthographicCamera::OrthographicCamera(float zNear, float zFar, float viewportWidth, float viewportHeight)
        : Camera{zNear, zFar, viewportWidth, viewportHeight}
    {
    }

    glm::mat4 OrthographicCamera::GetProjectionTransformation() const
    {
        const Rect nearPlaneGeometry{GetNearPlaneGeometry()};
        return glm::ortho(nearPlaneGeometry.GetLeftX(), nearPlaneGeometry.GetRightX(),
                          nearPlaneGeometry.GetBottomY(), nearPlaneGeometry.GetTopY(), GetZNear(), GetZFar());
    }

    Rect OrthographicCamera::GetNearPlaneGeometry() const
    {
        return Rect{GetPosition(), GetViewportWidth(), GetViewportHeight()};
    }

    Rect OrthographicCamera::GetFarPlaneGeometry() const { return GetNearPlaneGeometry(); }

    PerspectiveCamera::PerspectiveCamera(float zNear, float zFar, float viewportWidth, float viewportHeight,
                                         float fovY)
        : Camera{zNear, zFar, viewportWidth, viewportHeight}, m_fovY{fovY}
    {
    }

    glm::mat4 PerspectiveCamera::GetProjectionTransformation() const
    {
        return glm::perspective(m_fovY, GetAspectRatio(), GetZNear(), GetZFar());
    }

    Rect PerspectiveCamera::GetNearPlaneGeometry() const
    {
        const float height{glm::tan(m_fovY / 2) * GetZNear() * 2};
        return Rect{GetPosition(), height * GetAspectRatio(), height};
    }

    Rect PerspectiveCamera::GetFarPlaneGeometry() const
    {
        const float height{glm::tan(m_fovY / 2) * GetZFar() * 2};
        return Rect{GetPosition(), height * GetAspectRatio(), height};
    }

} // namespace Engine
