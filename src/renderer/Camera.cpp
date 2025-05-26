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

    glm::mat4 Camera::getCameraTransformation() const
    {
        const glm::vec3 position{getModelTransformation()[3]};
        const glm::vec3 gaze{-(getModelTransformation() * glm::vec4{0.0f, 0.0f, 1.0f, 0.0f})};
        const glm::vec3 up{getModelTransformation() * glm::vec4{0.0f, 1.0f, 0.0f, 0.0f}};
        return glm::lookAt(position, position + gaze, up);
    }

    void Camera::setModelTransformation(const glm::mat4& modelTransformation)
    {
        m_modelTransformation = modelTransformation;
        m_position = modelTransformation[3];
    }

    OrthographicCamera::OrthographicCamera(float zNear, float zFar, float viewportWidth, float viewportHeight)
        : Camera{zNear, zFar, viewportWidth, viewportHeight}
    {
    }

    glm::mat4 OrthographicCamera::getProjectionTransformation() const
    {
        const Rect nearPlaneGeometry{getNearPlaneGeometry()};
        return glm::ortho(nearPlaneGeometry.getLeftX(), nearPlaneGeometry.getRightX(),
                          nearPlaneGeometry.getBottomY(), nearPlaneGeometry.getTopY(), getZNear(), getZFar());
    }

    Rect OrthographicCamera::getNearPlaneGeometry() const
    {
        return Rect{getPosition(), getViewportWidth(), getViewportHeight()};
    }

    Rect OrthographicCamera::getFarPlaneGeometry() const { return getNearPlaneGeometry(); }

    PerspectiveCamera::PerspectiveCamera(float zNear, float zFar, float viewportWidth, float viewportHeight,
                                         float fovY)
        : Camera{zNear, zFar, viewportWidth, viewportHeight}, m_fovY{fovY}
    {
    }

    glm::mat4 PerspectiveCamera::getProjectionTransformation() const
    {
        return glm::perspective(m_fovY, getAspectRatio(), getZNear(), getZFar());
    }

    Rect PerspectiveCamera::getNearPlaneGeometry() const
    {
        const float height{glm::tan(m_fovY / 2) * getZNear() * 2};
        return Rect{getPosition(), height * getAspectRatio(), height};
    }

    Rect PerspectiveCamera::getFarPlaneGeometry() const
    {
        const float height{glm::tan(m_fovY / 2) * getZFar() * 2};
        return Rect{getPosition(), height * getAspectRatio(), height};
    }

} // namespace Engine
