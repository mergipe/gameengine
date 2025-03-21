Level = {
    assets = {
        {
            type = "texture",
            id = "tilemap-texture",
            file = "tilemaps/jungle.png",
            min_filter = "nearest",
            mag_filter = "nearest",
            wrap_x = "repeat",
            wrap_y = "repeat",
        },
        {
            type = "texture",
            id = "chopper-texture",
            file = "textures/chopper-green-spritesheet.png",
            min_filter = "nearest",
            mag_filter = "nearest",
        },
        { type = "texture", id = "su27-texture", file = "textures/su27-spritesheet.png" },
        { type = "texture", id = "f22-texture", file = "textures/f22-spritesheet.png" },
        { type = "texture", id = "fw190-texture", file = "textures/fw190-spritesheet.png" },
    },
    tilemap = {
        map_file = "resources/tilemaps/jungle.map",
        texture_id = "tilemap-texture",
        map_rows = 20,
        map_cols = 25,
        tileset_cols = 10,
        tile_size = 32,
        scale = 4.0,
    },
    entities = {
        {
            components = {
                tag = { name = "player" },
                transform = {
                    position = { x = 484, y = 220 },
                    scale = { x = 2.0, y = 2.0 },
                },
                rigid_body = {
                    velocity = { x = 0.0, y = 0.0 },
                },
                sprite = {
                    texture_id = "chopper-texture",
                    width = 32,
                    height = 32,
                    z_index = 4,
                    has_fixed_position = false,
                    source_rect_x = 0,
                    source_rect_y = 0,
                },
                animation = {
                    frames_count = 2,
                    frames_per_second = 10,
                },
                box_collider = {
                    width = 32,
                    height = 25,
                    offset = { x = 0, y = 5 },
                },
                player_input = {},
                camera_follow = {},
            },
        },
        {
            components = {
                tag = { name = "enemy" },
                transform = {
                    position = { x = 1370, y = 600 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = { z = -10.0 },
                },
                rigid_body = {
                    velocity = { x = -0.01375, y = -0.0875 },
                },
                sprite = {
                    texture_id = "su27-texture",
                    width = 32,
                    height = 32,
                    z_index = 5,
                },
                animation = {
                    frames_count = 2,
                    frames_per_second = 10,
                },
                box_collider = {
                    width = 25,
                    height = 30,
                    offset = { x = 5, y = 0 },
                },
            },
        },
        {
            components = {
                tag = { name = "enemy" },
                transform = {
                    position = { x = 1376, y = 330 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = { z = 45.0 },
                },
                sprite = {
                    texture_id = "f22-texture",
                    width = 32,
                    height = 32,
                    z_index = 4,
                },
                animation = {
                    frames_count = 2,
                    frames_per_second = 10,
                },
                box_collider = {
                    width = 20,
                    height = 25,
                    offset = { x = 5, y = 5 },
                },
            },
        },
        {
            components = {
                tag = { name = "enemy" },
                transform = {
                    position = { x = 2000, y = 286 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = { z = -90.0 },
                },
                rigid_body = {
                    velocity = { x = -0.125, y = 0.0 },
                },
                sprite = {
                    texture_id = "fw190-texture",
                    width = 32,
                    height = 32,
                    z_index = 6,
                },
                animation = {
                    frames_count = 3,
                    frames_per_second = 15,
                },
                box_collider = {
                    width = 32,
                    height = 30,
                    offset = { x = 0, y = 0 },
                },
            },
        },
    },
}
