#include "Components.h"
#include <ECS.h>
#include <Game.h>
#include <Logger.h>
#include <SDL.h>
#include <Systems.h>
#include <memory>

void Game::init() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        Logger::critical("Error initializing SDL: {}", SDL_GetError());
        return;
    }
    window = SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight,
                              SDL_WINDOW_BORDERLESS);
    if (!window) {
        Logger::critical("Error creating SDL window: {}", SDL_GetError());
        return;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        Logger::critical("Error creating SDL renderer: {}", SDL_GetError());
        return;
    }
    Logger::trace("Game initialized");
}

void Game::setup() {
    registry = std::make_unique<Registry>();
    registry->addSystem<MovementSystem>();
    registry->addSystem<RenderSystem>(renderer);
    assetStore = std::make_unique<AssetStore>("../../assets/", renderer);
    assetStore->addTexture("tank-right", "images/tank-panther-right.png");
    assetStore->addTexture("truck-right", "images/truck-ford-right.png");
    Entity e1{registry->createEntity()};
    registry->addComponent<TransformComponent>(e1, glm::vec2(10, 10), glm::vec2(1, 1), 0.0);
    registry->addComponent<RigidBodyComponent>(e1, glm::vec2(0.1, 0.1));
    registry->addComponent<SpriteComponent>(e1, "tank-right", 32, 32);
    Entity e2{registry->createEntity()};
    registry->addComponent<TransformComponent>(e2, glm::vec2(50, 50), glm::vec2(1, 1), 0.0);
    registry->addComponent<RigidBodyComponent>(e2, glm::vec2(0.2, 0.1));
    registry->addComponent<SpriteComponent>(e2, "tank-left", 64, 64);
}

void Game::run() {
    Logger::trace("Game started to run");
    setup();
    std::uint64_t previousTicks{SDL_GetTicks64()};
    float lag{0.0f};
    isRunning = true;
    while (isRunning) {
        std::uint64_t currentTicks{SDL_GetTicks64()};
        lag += static_cast<float>(currentTicks - previousTicks);
        previousTicks = currentTicks;
        processInput();
        while (lag >= timeStepInMs) {
            update();
            lag -= timeStepInMs;
        }
        render(lag);
    }
}

void Game::processInput() {
    SDL_Event event{};
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            isRunning = false;
            break;
        }
    }
}

void Game::update() {
    registry->update();
    registry->getSystem<MovementSystem>().update(timeStepInMs);
}

void Game::render(float frameExtrapolationTimeStep) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    registry->getSystem<RenderSystem>().update(frameExtrapolationTimeStep);
    SDL_RenderPresent(renderer);
}

void Game::destroy() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    Logger::trace("Game resources destroyed");
}
