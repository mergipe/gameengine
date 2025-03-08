Level = {
    assets = {
        { type = "texture", id = "tilemap-texture", file = "./assets/tilemaps/jungle.png" },
        { type = "texture", id = "chopper-texture", file = "./assets/textures/chopper-spritesheet.png" },
        { type = "texture", id = "tank-texture", file = "./assets/textures/tank-tiger-right.png" },
        { type = "texture", id = "bullet-texture", file = "./assets/textures/bullet.png" },
        { type = "font", id = "pico8-font-5", file = "./assets/fonts/pico8.ttf", font_size = 5 },
        { type = "font", id = "pico8-font-10", file = "./assets/fonts/pico8.ttf", font_size = 10 },
    },
    tilemap = {
        map_file = "./assets/tilemaps/jungle.map",
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
                player = {},
                transform = {
                    position = { x = 242, y = 110 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0,
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
                    frames_per_second = 15,
                },
                box_collider = {
                    width = 32,
                    height = 25,
                    offset = { x = 0, y = 5 },
                },
                player_input = {},
                camera_follow = {},
                health = {
                    health_percentage = 100,
                },
                projectile_emitter = {
                    projectile_velocity = { x = 0.4, y = 0.4 },
                    projectile_duration = 10,
                    cooldown = 0.2,
                    hit_percentage_damage = 10,
                    is_projectile_friendly = true,
                    is_auto_shoot = false,
                },
            },
        },
        {
            components = {
                enemy = {},
                transform = {
                    position = { x = 200, y = 497 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0,
                },
                sprite = {
                    texture_id = "tank-texture",
                    width = 32,
                    height = 32,
                    z_index = 2,
                },
                box_collider = {
                    width = 25,
                    height = 18,
                    offset = { x = 0, y = 7 },
                },
                health = {
                    health_percentage = 100,
                },
                projectile_emitter = {
                    projectile_velocity = { x = 0.2, y = 0 },
                    projectile_duration = 2,
                    cooldown = 1,
                    hit_percentage_damage = 20,
                    is_projectile_friendly = false,
                    is_auto_shoot = true,
                },
            },
        },
    },
}
