#ifndef SCENEDATA_H
#define SCENEDATA_H

#include "renderer/Camera.h"

namespace Engine
{
    struct SceneData {
        Camera camera;
        int mapWidth;
        int mapHeight;
    };
} // namespace Engine

#endif
