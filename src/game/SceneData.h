#ifndef SCENEDATA_H
#define SCENEDATA_H

#include "renderer/Camera.h"

namespace Engine
{
    struct MapData {
        float width;
        float height;
    };

    struct SceneData {
        Camera camera{};
        MapData mapData{};
    };
} // namespace Engine

#endif
