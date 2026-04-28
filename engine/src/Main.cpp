#include "Engine.h"

int main(int, char*[])
{
    Engine::Engine& engine{Engine::Engine::Instance()};
    engine.Start();
    return 0;
}
