#ifndef SCENEDATA_H
#define SCENEDATA_H

#include "LevelLoader.h"
#include "renderer/Camera.h"

namespace Engine
{
    struct SceneData {
        Camera camera;
        LevelData levelData;
    };
} // namespace Engine

#endif
