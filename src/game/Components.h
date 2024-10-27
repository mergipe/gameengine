#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <SDL.h>
#include <glm/glm.hpp>
#include <string>

namespace Engine
{
    struct TransformComponent {
        TransformComponent(const glm::vec2& position = glm::vec2(0), const glm::vec2& scale = glm::vec2(1),
                           double rotation = 0.0)
            : position{position}, scale{scale}, rotation{rotation}
        {
        }
        glm::vec2 position{};
        glm::vec2 scale{};
        double rotation{};
    };

    struct RigidBodyComponent {
        RigidBodyComponent(const glm::vec2& velocity = glm::vec2(0))
            : velocity{velocity}
        {
        }
        glm::vec2 velocity{};
    };

    struct SpriteComponent {
        SpriteComponent(const std::string& resourceId = "", int width = 0, int height = 0, int zIndex = 0,
                        int sourceRectX = 0, int sourceRectY = 0)
            : resourceId{resourceId}
            , sourceRect{SDL_Rect{sourceRectX, sourceRectY, width, height}}
            , width{width}
            , height{height}
            , zIndex{zIndex}
        {
        }
        std::string resourceId{};
        SDL_Rect sourceRect{};
        int width{};
        int height{};
        int zIndex{};
    };

    struct AnimationComponent {
        AnimationComponent(int framesCount = 1, int framesPerSecond = 1, bool shouldLoop = true)
            : framesCount{framesCount}, framesPerSecond{framesPerSecond}, shouldLoop{shouldLoop}
        {
        }
        std::uint64_t startTime{SDL_GetTicks64()};
        int currentFrame{0};
        int framesCount{};
        int framesPerSecond{};
        bool shouldLoop{};
    };

    struct BoxColliderComponent {
        BoxColliderComponent(int width = 0, int height = 0, const glm::vec2& offset = glm::vec2(0))
            : offset{offset}, width{width}, height{height}
        {
        }
        glm::vec2 offset{};
        int width{};
        int height{};
        bool isColliding{};
    };
} // namespace Engine

#endif
