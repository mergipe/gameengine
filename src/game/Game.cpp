#include <ECS.h>
#include <Game.h>
#include <IO.h>
#include <Logger.h>
#include <SDL.h>
#include <Systems.h>
#include <fstream>
#include <memory>
#include <string>

namespace Engine {

Game::Game(bool debugCapability) : debugCapability{debugCapability} {}

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

void Game::loadMap(std::string_view tilesetFilename, std::string_view tilemapFilename, int tileWidth,
                   int tileHeight, int tilesetColumns, float scale) {
    Logger::trace("Loading map {}", tilemapFilename);
    std::string tilemapsFolder{"tilemaps/"};
    std::string tilesetFilepath{tilemapsFolder + std::string{tilesetFilename}};
    resourceManager->addTexture("tileset", std::string{tilesetFilepath});
    std::string tilemapFilepath{std::string{resourceManager->getResourcesBasePath()} + tilemapsFolder +
                                std::string{tilemapFilename}};
    std::ifstream tilemapFile{tilemapFilepath};
    if (!tilemapFile) {
        Logger::error("Error opening {} file for reading", tilemapFilepath);
    }
    std::vector<std::vector<int>> tilemap{IO::parseIntCsvFile(tilemapFile)};
    tilemapFile.close();
    for (size_t i{0}; i < tilemap.size(); ++i) {
        std::vector<int> values{tilemap[i]};
        for (size_t j{0}; j < values.size(); ++j) {
            Entity tile{registry->createEntity()};
            registry->addComponent<TransformComponent>(
                tile,
                glm::vec2(static_cast<float>(j) * static_cast<float>(tileWidth) * scale,
                          static_cast<float>(i) * static_cast<float>(tileHeight) * scale),
                glm::vec2(scale, scale));
            int tileId{values[j]};
            registry->addComponent<SpriteComponent>(tile, "tileset", tileWidth, tileHeight, 0,
                                                    tileWidth * (tileId % tilesetColumns),
                                                    tileHeight * (tileId / tilesetColumns));
        }
    }
}

void Game::loadEntities() {
    resourceManager->addTexture("chopper", "images/chopper.png");
    resourceManager->addTexture("radar", "images/radar.png");
    Entity chopper{registry->createEntity()};
    registry->addComponent<TransformComponent>(chopper, glm::vec2(10, 10));
    registry->addComponent<RigidBodyComponent>(chopper, glm::vec2(0.0, 0.0));
    registry->addComponent<SpriteComponent>(chopper, "chopper", 32, 32, 1);
    registry->addComponent<AnimationComponent>(chopper, 2, 15);
    registry->addComponent<BoxColliderComponent>(chopper, 32, 32);
    Entity radar{registry->createEntity()};
    registry->addComponent<TransformComponent>(radar, glm::vec2(400, 10));
    registry->addComponent<SpriteComponent>(radar, "radar", 64, 64, 2);
    registry->addComponent<AnimationComponent>(radar, 8, 5);
    resourceManager->addTexture("tank-right", "images/tank-panther-right.png");
    resourceManager->addTexture("truck-right", "images/truck-ford-right.png");
    Entity tank{registry->createEntity()};
    registry->addComponent<TransformComponent>(tank, glm::vec2(10, 10), glm::vec2(2));
    registry->addComponent<RigidBodyComponent>(tank, glm::vec2(0.1, 0.1));
    registry->addComponent<SpriteComponent>(tank, "tank-right", 32, 32, 1);
    registry->addComponent<BoxColliderComponent>(tank, 32, 32);
    Entity truck{registry->createEntity()};
    registry->addComponent<TransformComponent>(truck, glm::vec2(50, 50));
    registry->addComponent<RigidBodyComponent>(truck, glm::vec2(0.05, 0.05));
    registry->addComponent<SpriteComponent>(truck, "truck-right", 32, 32, 1);
    registry->addComponent<BoxColliderComponent>(truck, 32, 32);
    registry->killEntity(truck);
}

void Game::loadLevel(int level) {
    Logger::trace("Loading level {}", level);
    registry = std::make_unique<Registry>();
    registry->addSystem<MovementSystem>();
    registry->addSystem<RenderSystem>(renderer, resourceManager);
    registry->addSystem<AnimationSystem>();
    registry->addSystem<CollisionSystem>();
    if (debugCapability) {
        registry->addSystem<DebugRenderSystem>(renderer);
    }
    resourceManager =
        std::make_unique<ResourceManager>("/home/gustavo/workspaces/gamedev/gameengine/resources/", renderer);
    loadMap("jungle.png", "jungle.map", 32, 32, 10, 2.0);
    loadEntities();
}

void Game::setup() { loadLevel(1); }

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
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_F1 && debugCapability) {
                debugModeActivated = !debugModeActivated;
            }
            break;
        }
    }
}

void Game::update() {
    registry->update();
    registry->getSystem<MovementSystem>().update(timeStepInMs);
    registry->getSystem<CollisionSystem>().update();
    registry->getSystem<AnimationSystem>().update();
}

void Game::render(float frameExtrapolationTimeStep) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    registry->getSystem<RenderSystem>().update(frameExtrapolationTimeStep);
    if (debugModeActivated) {
        registry->getSystem<DebugRenderSystem>().update(frameExtrapolationTimeStep);
    }
    SDL_RenderPresent(renderer);
}

void Game::destroy() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    Logger::trace("Game resources destroyed");
}

} // namespace Engine