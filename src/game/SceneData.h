#ifndef SCENEDATA_H
#define SCENEDATA_H

namespace Engine
{
    struct MapData {
        float width;
        float height;
    };

    struct SceneData {
        MapData mapData{};
    };
} // namespace Engine

#endif
