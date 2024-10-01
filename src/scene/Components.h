#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <SDL.h>
#include <glm/glm.hpp>
#include <string_view>

struct TransformComponent {
    glm::vec2 position{};
    glm::vec2 scale{};
    double rotation{};

    TransformComponent(glm::vec2 position = glm::vec2(0), glm::vec2 scale = glm::vec2(1),
                       double rotation = 0.0)
        : position{position}, scale{scale}, rotation{rotation} {}
};

struct RigidBodyComponent {
    glm::vec2 velocity{};

    RigidBodyComponent(glm::vec2 velocity = glm::vec2(0)) : velocity{velocity} {}
};

struct SpriteComponent {
    const std::string_view assetId{};
    const int width{};
    const int height{};
    const int zIndex{};
    SDL_Rect sourceRect{};

    SpriteComponent(std::string_view assetId = "", int width = 0, int height = 0, int zIndex = 0,
                    int rectX = 0, int rectY = 0)
        : assetId{assetId}, width{width}, height{height}, zIndex{zIndex},
          sourceRect{SDL_Rect{rectX, rectY, width, height}} {}
};

struct AnimationComponent {
    const int framesCount{};
    int currentFrame{0};
    const int framesPerSecond{};
    const bool shouldLoop{};
    const std::uint64_t startTime{SDL_GetTicks64()};

    AnimationComponent(int framesCount = 1, int framesPerSecond = 1, bool shouldLoop = true)
        : framesCount{framesCount}, framesPerSecond{framesPerSecond}, shouldLoop{shouldLoop} {}
};

struct BoxColliderComponent {
    const int width{};
    const int height{};
    const glm::vec2 offset{};

    BoxColliderComponent(int width = 0, int height = 0, glm::vec2 offset = glm::vec2(0))
        : width{width}, height{height}, offset{offset} {}
};

#endif
