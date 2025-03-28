Scene = {
    assets = {
        {
            type = "texture",
            id = "spritesheet-texture",
            file = "textures/space-shooter-spritesheet.png",
            min_filter = "linear",
            mag_filter = "linear",
            wrap_x = "repeat",
            wrap_y = "repeat",
        },
        {
            type = "texture",
            id = "planet-texture",
            file = "textures/planet00.png",
            min_filter = "linear",
            mag_filter = "linear",
        },
        {
            type = "texture",
            id = "tilemap-texture",
            file = "tilemaps/jungle.png",
            min_filter = "nearest",
            mag_filter = "nearest",
        },
    },
    tilemap = {
        map_file = "resources/tilemaps/jungle.map",
        texture_id = "tilemap-texture",
        map_rows = 20,
        map_cols = 25,
        tileset_cols = 10,
        tile_size = 32,
        scale = 6.0,
    },
    entities = {
        {
            components = {
                tag = { name = "player" },
                transform = {
                    position = { w = 484, y = 220 },
                    scale = { x = 0.7, y = 0.7 },
                },
                rigid_body = {
                    velocity = { x = 0.0, y = 0.0 },
                },
                sprite = {
                    texture_id = "spritesheet-texture",
                    texture_x = 1365,
                    texture_y = 1696,
                    width = 198,
                    height = 188,
                    z_index = 4,
                    has_fixed_position = false,
                },
                -- animation = {
                --     frames_count = 2,
                --     frames_per_second = 10,
                -- },
                box_collider = {
                    width = 198,
                    height = 188,
                    offset = { x = 0, y = 0 },
                },
                player_input = {},
            },
        },
        {
            components = {
                tag = { name = "planet" },
                transform = {
                    position = { x = 1260, y = 450 },
                    scale = { x = 1.0, y = 1.0 },
                },
                sprite = {
                    texture_id = "planet-texture",
                    width = 1280,
                    height = 1280,
                    z_index = 1,
                    has_fixed_position = false,
                    source_rect_x = 0,
                    source_rect_y = 0,
                },
                box_collider = {
                    width = 1280,
                    height = 1280,
                },
            },
        },
        -- {
        --     components = {
        --         tag = { name = "enemy" },
        --         transform = {
        --             position = { x = 1370, y = 600 },
        --             scale = { x = 2.0, y = 2.0 },
        --             rotation = { z = -10.0 },
        --         },
        --         rigid_body = {
        --             velocity = { x = -0.01375, y = -0.0875 },
        --         },
        --         sprite = {
        --             texture_id = "su27-texture",
        --             width = 32,
        --             height = 32,
        --             z_index = 5,
        --         },
        --         animation = {
        --             frames_count = 2,
        --             frames_per_second = 10,
        --         },
        --         box_collider = {
        --             width = 25,
        --             height = 30,
        --             offset = { x = 5, y = 0 },
        --         },
        --     },
        -- },
        -- {
        --     components = {
        --         tag = { name = "enemy" },
        --         transform = {
        --             position = { x = 1376, y = 330 },
        --             scale = { x = 2.0, y = 2.0 },
        --             rotation = { z = 45.0 },
        --         },
        --         sprite = {
        --             texture_id = "f22-texture",
        --             width = 32,
        --             height = 32,
        --             z_index = 4,
        --         },
        --         animation = {
        --             frames_count = 2,
        --             frames_per_second = 10,
        --         },
        --         box_collider = {
        --             width = 20,
        --             height = 25,
        --             offset = { x = 5, y = 5 },
        --         },
        --     },
        -- },
        -- {
        --     components = {
        --         tag = { name = "enemy" },
        --         transform = {
        --             position = { x = 2000, y = 286 },
        --             scale = { x = 2.0, y = 2.0 },
        --             rotation = { z = -90.0 },
        --         },
        --         rigid_body = {
        --             velocity = { x = -0.125, y = 0.0 },
        --         },
        --         sprite = {
        --             texture_id = "fw190-texture",
        --             width = 32,
        --             height = 32,
        --             z_index = 6,
        --         },
        --         animation = {
        --             frames_count = 3,
        --             frames_per_second = 15,
        --         },
        --         box_collider = {
        --             width = 32,
        --             height = 30,
        --             offset = { x = 0, y = 0 },
        --         },
        --     },
        -- },
    },
}
