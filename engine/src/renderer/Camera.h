#ifndef CAMERA_H
#define CAMERA_H

#include "Shapes.h"

#include <glm/glm.hpp>
#include <optional>
#include <string_view>

namespace Engine
{
    enum class ProjectionType { orthographic, perspective };

    constexpr std::optional<ProjectionType> ParseProjectionType(std::string_view projectionTypeStr)
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
        virtual glm::mat4 GetProjectionTransformation() const = 0;
        virtual Rect GetNearPlaneGeometry() const = 0;
        virtual Rect GetFarPlaneGeometry() const = 0;
        const glm::mat4& GetModelTransformation() const { return m_modelTransformation; }
        const glm::vec3& GetPosition() const { return m_position; }
        float GetZNear() const { return m_zNear; }
        float GetZFar() const { return m_zFar; }
        float GetViewportWidth() const { return m_viewportWidth; }
        float GetViewportHeight() const { return m_viewportHeight; }
        float GetAspectRatio() const { return m_aspectRatio; }
        glm::mat4 GetCameraTransformation() const;
        void SetModelTransformation(const glm::mat4& modelTransformation);

    private:
        glm::mat4 m_modelTransformation{};
        glm::vec3 m_position{};
        float m_zNear{};
        float m_zFar{};
        float m_viewportWidth{};
        float m_viewportHeight{};
        float m_aspectRatio{};
    };

    class OrthographicCamera final : public Camera
    {
    public:
        OrthographicCamera(float zNear, float zFar, float viewportWidth, float viewportHeight);
        glm::mat4 GetProjectionTransformation() const override;
        Rect GetNearPlaneGeometry() const override;
        Rect GetFarPlaneGeometry() const override;
    };

    class PerspectiveCamera final : public Camera
    {
    public:
        PerspectiveCamera(float zNear, float zFar, float viewportWidth, float viewportHeight, float fovY);
        glm::mat4 GetProjectionTransformation() const override;
        Rect GetNearPlaneGeometry() const override;
        Rect GetFarPlaneGeometry() const override;

    private:
        float m_fovY{};
    };
} // namespace Engine

#endif
