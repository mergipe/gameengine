#ifndef SCENE_DATA_H
#define SCENE_DATA_H

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
