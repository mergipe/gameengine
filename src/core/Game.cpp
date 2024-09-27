#include <ECS.h>
#include <Game.h>
#include <Logger.h>
#include <SDL.h>

Game::Game() { registry = std::make_unique<Registry>(); }

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

void setup() {}

void Game::run() {
    Logger::trace("Game started to run");
    setup();
    std::uint64_t previousTicks{SDL_GetTicks64()};
    int lag{0};
    isRunning = true;
    while (isRunning) {
        std::uint64_t currentTicks{SDL_GetTicks64()};
        lag += static_cast<int>(currentTicks - previousTicks);
        previousTicks = currentTicks;
        processInput();
        while (lag >= timeStepInMs) {
            update(timeStepInMs);
            lag -= timeStepInMs;
        }
        render(static_cast<float>(lag) / timeStepInMs);
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

void Game::update(int timeStep) { (void)timeStep; }

void Game::render(float frameExtrapolationFactor) {
    (void)frameExtrapolationFactor;
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}

void Game::destroy() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    Logger::trace("Game resources destroyed");
}
