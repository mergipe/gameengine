#ifndef CAMERA_H
#define CAMERA_H

#include "Shapes.h"
#include <glm/glm.hpp>
#include <optional>
#include <string_view>

namespace Engine
{
    enum class ProjectionType { orthographic, perspective };

    constexpr std::optional<ProjectionType> getProjectionType(std::string_view projectionTypeStr)
    {
        using enum ProjectionType;
        if (projectionTypeStr == "orthographic")
            return orthographic;
        if (projectionTypeStr == "perspective")
            return perspective;
        return {};
    }

    class Camera
    {
    public:
        Camera(float zNear, float zFar, float viewportWidth, float viewportHeight);
        Camera(const Camera&) = delete;
        Camera(Camera&&) = delete;
        Camera& operator=(const Camera&) = delete;
        Camera& operator=(Camera&&) = delete;
        virtual ~Camera() = default;
        virtual glm::mat4 getProjectionTransformation() const = 0;
        virtual Rect getNearPlaneGeometry() const = 0;
        virtual Rect getFarPlaneGeometry() const = 0;
        const glm::mat4& getModelTransformation() const { return m_modelTransformation; }
        const glm::vec3& getPosition() const { return m_position; }
        float getZNear() const { return m_zNear; }
        float getZFar() const { return m_zFar; }
        float getViewportWidth() const { return m_viewportWidth; }
        float getViewportHeight() const { return m_viewportHeight; }
        float getAspectRatio() const { return m_aspectRatio; }
        glm::mat4 getCameraTransformation() const;
        void setModelTransformation(const glm::mat4& modelTransformation);

    private:
        glm::mat4 m_modelTransformation{};
        glm::vec3 m_position{};
        float m_zNear{};
        float m_zFar{};
        float m_viewportWidth{};
        float m_viewportHeight{};
        float m_aspectRatio{};
    };

    class OrthographicCamera : public Camera
    {
    public:
        OrthographicCamera(float zNear, float zFar, float viewportWidth, float viewportHeight);
        glm::mat4 getProjectionTransformation() const override;
        Rect getNearPlaneGeometry() const override;
        Rect getFarPlaneGeometry() const override;
    };

    class PerspectiveCamera : public Camera
    {
    public:
        PerspectiveCamera(float zNear, float zFar, float viewportWidth, float viewportHeight, float fovY);
        glm::mat4 getProjectionTransformation() const override;
        Rect getNearPlaneGeometry() const override;
        Rect getFarPlaneGeometry() const override;

    private:
        float m_fovY{};
    };
} // namespace Engine

#endif
