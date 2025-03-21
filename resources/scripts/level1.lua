local current_system_hour = os.date("*t").hour
local map_texture_id
if current_system_hour >= 9 and current_system_hour < 18 then
    map_texture_id = "tilemap-texture-day"
else
    map_texture_id = "tilemap-texture-night"
end

Level = {
    assets = {
        { type = "texture", id = "tilemap-texture-day", file = "resources/tilemaps/jungle.png" },
        { type = "texture", id = "tilemap-texture-night", file = "resources/tilemaps/jungle-night.png" },
        {
            type = "texture",
            id = "chopper-texture",
            file = "resources/textures/chopper-green-spritesheet.png",
        },
        { type = "texture", id = "su27-texture", file = "resources/textures/su27-spritesheet.png" },
        { type = "texture", id = "f22-texture", file = "resources/textures/f22-spritesheet.png" },
        { type = "texture", id = "fw190-texture", file = "resources/textures/fw190-spritesheet.png" },
        { type = "texture", id = "upf7-texture", file = "resources/textures/upf7-spritesheet.png" },
        { type = "texture", id = "bf109-texture", file = "resources/textures/bf109-spritesheet.png" },
        { type = "texture", id = "bomber-texture", file = "resources/textures/bomber-spritesheet.png" },
        { type = "texture", id = "carrier-texture", file = "resources/textures/carrier.png" },
        { type = "texture", id = "boat-texture", file = "resources/textures/boat.png" },
        { type = "texture", id = "tank-tiger-up-texture", file = "resources/textures/tank-tiger-up.png" },
        { type = "texture", id = "tank-tiger-right-texture", file = "resources/textures/tank-tiger-right.png" },
        { type = "texture", id = "tank-tiger-down-texture", file = "resources/textures/tank-tiger-down.png" },
        { type = "texture", id = "tank-tiger-left-texture", file = "resources/textures/tank-tiger-left.png" },
        { type = "texture", id = "tank-tiger-killed-texture", file = "resources/textures/tank-tiger-killed.png" },
        { type = "texture", id = "tank-panther-up-texture", file = "resources/textures/tank-panther-up.png" },
        { type = "texture", id = "tank-panther-right-texture", file = "resources/textures/tank-panther-right.png" },
        { type = "texture", id = "tank-panther-down-texture", file = "resources/textures/tank-panther-down.png" },
        { type = "texture", id = "tank-panther-left-texture", file = "resources/textures/tank-panther-left.png" },
        { type = "texture", id = "tank-panther-killed-texture", file = "resources/textures/tank-panther-killed.png" },
        { type = "texture", id = "truck-ford-up-texture", file = "resources/textures/truck-ford-up.png" },
        { type = "texture", id = "truck-ford-right-texture", file = "resources/textures/truck-ford-right.png" },
        { type = "texture", id = "truck-ford-down-texture", file = "resources/textures/truck-ford-down.png" },
        { type = "texture", id = "truck-ford-left-texture", file = "resources/textures/truck-ford-left.png" },
        { type = "texture", id = "truck-ford-killed-texture", file = "resources/textures/truck-ford-killed.png" },
        { type = "texture", id = "army-walk-up-texture", file = "resources/textures/army-walk-up.png" },
        { type = "texture", id = "army-walk-right-texture", file = "resources/textures/army-walk-right.png" },
        { type = "texture", id = "army-walk-down-texture", file = "resources/textures/army-walk-down.png" },
        { type = "texture", id = "army-walk-left-texture", file = "resources/textures/army-walk-left.png" },
        { type = "texture", id = "army-walk-killed-texture", file = "resources/textures/army-walk-killed.png" },
        { type = "texture", id = "army-gun-up-texture", file = "resources/textures/army-gun-up.png" },
        { type = "texture", id = "army-gun-right-texture", file = "resources/textures/army-gun-right.png" },
        { type = "texture", id = "army-gun-down-texture", file = "resources/textures/army-gun-down.png" },
        { type = "texture", id = "army-gun-left-texture", file = "resources/textures/army-gun-left.png" },
        -- { type = "texture", id = "army-gun-killed-texture", file = "resources/textures/army-gun-killed.png" },
        { type = "texture", id = "sam-truck-right-texture", file = "resources/textures/sam-truck-right.png" },
        {
            type = "texture",
            id = "sam-tank-left-texture",
            file = "resources/textures/sam-tank-left-spritesheet.png",
        },
        {
            type = "texture",
            id = "sam-tank-right-texture",
            file = "resources/textures/sam-tank-right-spritesheet.png",
        },
        { type = "texture", id = "takeoff-base-texture", file = "resources/textures/takeoff-base.png" },
        { type = "texture", id = "landing-base-texture", file = "resources/textures/landing-base.png" },
        { type = "texture", id = "runway-texture", file = "resources/textures/runway.png" },
        { type = "texture", id = "obstacles1-texture", file = "resources/textures/obstacles-1.png" },
        { type = "texture", id = "obstacles2-texture", file = "resources/textures/obstacles-2.png" },
        { type = "texture", id = "obstacles3-texture", file = "resources/textures/obstacles-3.png" },
        { type = "texture", id = "obstacles4-texture", file = "resources/textures/obstacles-4.png" },
        { type = "texture", id = "obstacles5-texture", file = "resources/textures/obstacles-5.png" },
        { type = "texture", id = "obstacles6-texture", file = "resources/textures/obstacles-6.png" },
        { type = "texture", id = "obstacles7-texture", file = "resources/textures/obstacles-7.png" },
        { type = "texture", id = "tree1-texture", file = "resources/textures/tree-1.png" },
        { type = "texture", id = "tree2-texture", file = "resources/textures/tree-2.png" },
        { type = "texture", id = "tree3-texture", file = "resources/textures/tree-3.png" },
        { type = "texture", id = "tree4-texture", file = "resources/textures/tree-4.png" },
        { type = "texture", id = "tree5-texture", file = "resources/textures/tree-5.png" },
        { type = "texture", id = "tree6-texture", file = "resources/textures/tree-6.png" },
        { type = "texture", id = "tree7-texture", file = "resources/textures/tree-7.png" },
        { type = "texture", id = "tree8-texture", file = "resources/textures/tree-8.png" },
        { type = "texture", id = "tree9-texture", file = "resources/textures/tree-9.png" },
        { type = "texture", id = "tree10-texture", file = "resources/textures/tree-10.png" },
        { type = "texture", id = "tree11-texture", file = "resources/textures/tree-11.png" },
        { type = "texture", id = "tree12-texture", file = "resources/textures/tree-12.png" },
        { type = "texture", id = "tree13-texture", file = "resources/textures/tree-13.png" },
        { type = "texture", id = "tree14-texture", file = "resources/textures/tree-14.png" },
        { type = "texture", id = "tree15-texture", file = "resources/textures/tree-15.png" },
        { type = "texture", id = "tree16-texture", file = "resources/textures/tree-16.png" },
        { type = "texture", id = "tree17-texture", file = "resources/textures/tree-17.png" },
        { type = "texture", id = "tree18-texture", file = "resources/textures/tree-18.png" },
        { type = "texture", id = "tree19-texture", file = "resources/textures/tree-19.png" },
        { type = "texture", id = "tree20-texture", file = "resources/textures/tree-20.png" },
        { type = "texture", id = "bullet-texture", file = "resources/textures/bullet.png" },
        { type = "texture", id = "radar-texture", file = "resources/textures/radar-spritesheet.png" },
        { type = "font", id = "pico8-font-5", file = "resources/fonts/pico8.ttf", font_size = 5 },
        { type = "font", id = "pico8-font-10", file = "resources/fonts/pico8.ttf", font_size = 10 },
    },
    tilemap = {
        map_file = "resources/tilemaps/jungle.map",
        texture_id = map_texture_id,
        map_rows = 20,
        map_cols = 25,
        tileset_cols = 10,
        tile_size = 32,
        scale = 4.0,
    },
    entities = {
        {
            -- Player
            components = {
                player = {},
                transform = {
                    position = { x = 484, y = 220 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
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
                    frames_per_second = 10, -- fps
                },
                box_collider = {
                    width = 32,
                    height = 25,
                    offset = { x = 0, y = 5 },
                },
                health = {
                    health_percentage = 100,
                },
                projectile_emitter = {
                    projectile_velocity = { x = 0.5, y = 0.5 },
                    projectile_duration = 10, -- seconds
                    cooldown = 0.1, -- seconds
                    hit_percentage_damage = 10,
                    is_projectile_friendly = true,
                    is_auto_shoot = false,
                },
                player_input = {},
                camera_follow = {},
            },
        },
        {
            -- Takeoff base
            components = {
                transform = {
                    position = { x = 480, y = 226 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "takeoff-base-texture",
                    width = 32,
                    height = 32,
                    z_index = 1,
                },
            },
        },
        {
            -- Landing base
            components = {
                transform = {
                    position = { x = 2792, y = 986 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "landing-base-texture",
                    width = 32,
                    height = 32,
                    z_index = 1,
                },
            },
        },
        {
            -- Radar UI animation
            components = {
                ui = {},
                transform = {
                    position = { x = 20, y = 20 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "radar-texture",
                    width = 64,
                    height = 64,
                    z_index = 10,
                    has_fixed_position = true,
                },
                animation = {
                    frames_count = 8,
                    frames_per_second = 7, -- fps
                },
            },
        },
        {
            -- Runway
            components = {
                transform = {
                    position = { x = 1880, y = 130 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 270.0, -- degrees
                },
                sprite = {
                    texture_id = "runway-texture",
                    width = 21,
                    height = 191,
                    z_index = 1,
                },
            },
        },
        {
            -- Runway
            components = {
                transform = {
                    position = { x = 940, y = 770 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "runway-texture",
                    width = 21,
                    height = 191,
                    z_index = 1,
                },
            },
        },
        {
            -- Tank
            components = {
                enemy = {},
                transform = {
                    position = { x = 400, y = 994 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "tank-tiger-right-texture",
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
                    projectile_velocity = { x = 0.25, y = 0 },
                    projectile_duration = 2, -- seconds
                    cooldown = 1, -- seconds
                    hit_percentage_damage = 20,
                    is_projectile_friendly = false,
                },
            },
        },
        {
            -- Tank
            components = {
                enemy = {},
                transform = {
                    position = { x = 1570, y = 340 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "tank-panther-up-texture",
                    width = 32,
                    height = 32,
                    z_index = 1,
                },
                box_collider = {
                    width = 17,
                    height = 18,
                    offset = { x = 7, y = 10 },
                },
                health = {
                    health_percentage = 100,
                },
                projectile_emitter = {
                    projectile_velocity = { x = 0, y = -0.125 },
                    projectile_duration = 4, -- seconds
                    cooldown = 2, -- seconds
                    hit_percentage_damage = 10,
                    is_projectile_friendly = false,
                },
            },
        },
        {
            -- Tank
            components = {
                enemy = {},
                transform = {
                    position = { x = 1570, y = 500 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "tank-tiger-down-texture",
                    width = 32,
                    height = 32,
                    z_index = 1,
                },
                box_collider = {
                    width = 20,
                    height = 18,
                    offset = { x = 5, y = 7 },
                },
                health = {
                    health_percentage = 100,
                },
                projectile_emitter = {
                    projectile_velocity = { x = 0, y = 0.05 },
                    projectile_duration = 3, -- seconds
                    cooldown = 3, -- seconds
                    hit_percentage_damage = 10,
                    is_projectile_friendly = false,
                },
            },
        },
        {
            -- Tank
            components = {
                enemy = {},
                transform = {
                    position = { x = 1570, y = 700 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "tank-panther-left-texture",
                    width = 32,
                    height = 32,
                    z_index = 1,
                },
                box_collider = {
                    width = 25,
                    height = 18,
                    offset = { x = 5, y = 7 },
                },
                health = {
                    health_percentage = 100,
                },
                projectile_emitter = {
                    projectile_velocity = { x = -0.125, y = 0 },
                    projectile_duration = 3, -- seconds
                    cooldown = 3, -- seconds
                    hit_percentage_damage = 10,
                    is_projectile_friendly = false,
                },
            },
        },
        {
            -- Tank
            components = {
                enemy = {},
                transform = {
                    position = { x = 1140, y = 1040 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "tank-panther-right-texture",
                    width = 32,
                    height = 32,
                    z_index = 1,
                },
                box_collider = {
                    width = 25,
                    height = 18,
                    offset = { x = 5, y = 7 },
                },
                health = {
                    health_percentage = 100,
                },
                projectile_emitter = {
                    projectile_velocity = { x = 0.15, y = 0 },
                    projectile_duration = 4, -- seconds
                    cooldown = 4, -- seconds
                    hit_percentage_damage = 10,
                    is_projectile_friendly = false,
                },
            },
        },
        {
            -- Tank
            components = {
                enemy = {},
                transform = {
                    position = { x = 1140, y = 1200 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "tank-panther-left-texture",
                    width = 32,
                    height = 32,
                    z_index = 1,
                },
                box_collider = {
                    width = 25,
                    height = 18,
                    offset = { x = 5, y = 7 },
                },
                health = {
                    health_percentage = 100,
                },
                projectile_emitter = {
                    projectile_velocity = { x = -0.15, y = 0 },
                    projectile_duration = 4, -- seconds
                    cooldown = 4, -- seconds
                    hit_percentage_damage = 10,
                    is_projectile_friendly = false,
                },
            },
        },
        {
            -- Tank
            components = {
                enemy = {},
                transform = {
                    position = { x = 2100, y = 340 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "tank-panther-right-texture",
                    width = 32,
                    height = 32,
                    z_index = 1,
                },
                box_collider = {
                    width = 25,
                    height = 18,
                    offset = { x = 5, y = 7 },
                },
                health = {
                    health_percentage = 100,
                },
                projectile_emitter = {
                    projectile_velocity = { x = 0.15, y = 0 },
                    projectile_duration = 4, -- seconds
                    cooldown = 4, -- seconds
                    hit_percentage_damage = 10,
                    is_projectile_friendly = false,
                },
            },
        },
        {
            -- Tank
            components = {
                enemy = {},
                transform = {
                    position = { x = 2340, y = 232 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "tank-panther-down-texture",
                    width = 32,
                    height = 32,
                    z_index = 1,
                },
                box_collider = {
                    width = 17,
                    height = 18,
                    offset = { x = 8, y = 6 },
                },
                health = {
                    health_percentage = 100,
                },
                projectile_emitter = {
                    projectile_velocity = { x = 0, y = 0.1 },
                    projectile_duration = 2, -- seconds
                    cooldown = 2, -- seconds
                    hit_percentage_damage = 10,
                    is_projectile_friendly = false,
                },
            },
        },
        {
            -- Tank
            components = {
                enemy = {},
                transform = {
                    position = { x = 2760, y = 232 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "tank-panther-down-texture",
                    width = 32,
                    height = 32,
                    z_index = 1,
                },
                box_collider = {
                    width = 17,
                    height = 18,
                    offset = { x = 8, y = 6 },
                },
                health = {
                    health_percentage = 100,
                },
                projectile_emitter = {
                    projectile_velocity = { x = 0, y = 0.1 },
                    projectile_duration = 2, -- seconds
                    cooldown = 2, -- seconds
                    hit_percentage_damage = 10,
                    is_projectile_friendly = false,
                },
            },
        },
        {
            -- Tank
            components = {
                enemy = {},
                transform = {
                    position = { x = 2530, y = 480 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "tank-panther-killed-texture",
                    width = 32,
                    height = 32,
                    z_index = 1,
                },
            },
        },
        {
            -- Tank
            components = {
                enemy = {},
                transform = {
                    position = { x = 2530, y = 580 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "tank-panther-left-texture",
                    width = 32,
                    height = 32,
                    z_index = 1,
                },
                box_collider = {
                    width = 20,
                    height = 17,
                    offset = { x = 7, y = 7 },
                },
                health = {
                    health_percentage = 100,
                },
                projectile_emitter = {
                    projectile_velocity = { x = -0.1, y = 0 },
                    projectile_duration = 2, -- seconds
                    cooldown = 2, -- seconds
                    hit_percentage_damage = 10,
                    is_projectile_friendly = false,
                },
            },
        },
        {
            -- Tank
            components = {
                enemy = {},
                transform = {
                    position = { x = 1280, y = 1600 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "tank-tiger-down-texture",
                    width = 32,
                    height = 32,
                    z_index = 1,
                },
                box_collider = {
                    width = 18,
                    height = 20,
                    offset = { x = 7, y = 7 },
                },
                health = {
                    health_percentage = 100,
                },
                projectile_emitter = {
                    projectile_velocity = { x = 0, y = 0.25 },
                    projectile_duration = 5, -- seconds
                    cooldown = 5, -- seconds
                    hit_percentage_damage = 10,
                    is_projectile_friendly = false,
                },
            },
        },
        {
            -- Tank
            components = {
                enemy = {},
                transform = {
                    position = { x = 1580, y = 1490 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "tank-tiger-left-texture",
                    width = 32,
                    height = 32,
                    z_index = 1,
                },
                box_collider = {
                    width = 25,
                    height = 18,
                    offset = { x = 7, y = 7 },
                },
                health = {
                    health_percentage = 100,
                },
                projectile_emitter = {
                    projectile_velocity = { x = -0.15, y = 0 },
                    projectile_duration = 10, -- seconds
                    cooldown = 1, -- seconds
                    hit_percentage_damage = 10,
                    is_projectile_friendly = false,
                },
            },
        },
        {
            -- Tank
            components = {
                enemy = {},
                transform = {
                    position = { x = 1960, y = 1580 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "tank-tiger-right-texture",
                    width = 32,
                    height = 32,
                    z_index = 1,
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
                    projectile_velocity = { x = 0.15, y = 0 },
                    projectile_duration = 2, -- seconds
                    cooldown = 1, -- seconds
                    hit_percentage_damage = 10,
                    is_projectile_friendly = false,
                },
            },
        },
        {
            -- Tank
            components = {
                enemy = {},
                transform = {
                    position = { x = 2140, y = 1740 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "tank-panther-down-texture",
                    width = 32,
                    height = 32,
                    z_index = 1,
                },
                box_collider = {
                    width = 17,
                    height = 20,
                    offset = { x = 8, y = 4 },
                },
                health = {
                    health_percentage = 100,
                },
                projectile_emitter = {
                    projectile_velocity = { x = 0, y = 0.25 },
                    projectile_duration = 4, -- seconds
                    cooldown = 2, -- seconds
                    hit_percentage_damage = 10,
                    is_projectile_friendly = false,
                },
            },
        },
        {
            -- Tank
            components = {
                enemy = {},
                transform = {
                    position = { x = 2380, y = 1580 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "tank-panther-up-texture",
                    width = 32,
                    height = 32,
                    z_index = 1,
                },
                box_collider = {
                    width = 17,
                    height = 20,
                    offset = { x = 7, y = 8 },
                },
                health = {
                    health_percentage = 100,
                },
                projectile_emitter = {
                    projectile_velocity = { x = 0, y = -0.5 },
                    projectile_duration = 2, -- seconds
                    cooldown = 1, -- seconds
                    hit_percentage_damage = 10,
                    is_projectile_friendly = false,
                },
            },
        },
        {
            -- Tank
            components = {
                enemy = {},
                transform = {
                    position = { x = 2420, y = 1580 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "tank-panther-up-texture",
                    width = 32,
                    height = 32,
                    z_index = 1,
                },
                box_collider = {
                    width = 17,
                    height = 20,
                    offset = { x = 7, y = 8 },
                },
                health = {
                    health_percentage = 100,
                },
                projectile_emitter = {
                    projectile_velocity = { x = 0, y = -0.5 },
                    projectile_duration = 2, -- seconds
                    cooldown = 1, -- seconds
                    hit_percentage_damage = 10,
                    is_projectile_friendly = false,
                },
            },
        },
        {
            -- Tank
            components = {
                enemy = {},
                transform = {
                    position = { x = 2460, y = 1580 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "tank-panther-up-texture",
                    width = 32,
                    height = 32,
                    z_index = 1,
                },
                box_collider = {
                    width = 17,
                    height = 20,
                    offset = { x = 7, y = 8 },
                },
                health = {
                    health_percentage = 100,
                },
                projectile_emitter = {
                    projectile_velocity = { x = 0, y = -0.5 },
                    projectile_duration = 2, -- seconds
                    cooldown = 1, -- seconds
                    hit_percentage_damage = 10,
                    is_projectile_friendly = false,
                },
            },
        },
        {
            -- Tank
            components = {
                enemy = {},
                transform = {
                    position = { x = 2500, y = 1580 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "tank-panther-up-texture",
                    width = 32,
                    height = 32,
                    z_index = 1,
                },
                box_collider = {
                    width = 17,
                    height = 20,
                    offset = { x = 7, y = 8 },
                },
                health = {
                    health_percentage = 100,
                },
                projectile_emitter = {
                    projectile_velocity = { x = 0, y = -0.5 },
                    projectile_duration = 2, -- seconds
                    cooldown = 1, -- seconds
                    hit_percentage_damage = 10,
                    is_projectile_friendly = false,
                },
            },
        },
        {
            -- Tank
            components = {
                enemy = {},
                transform = {
                    position = { x = 2000, y = 890 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "tank-panther-up-texture",
                    width = 32,
                    height = 32,
                    z_index = 1,
                },
                box_collider = {
                    width = 17,
                    height = 20,
                    offset = { x = 7, y = 8 },
                },
                health = {
                    health_percentage = 100,
                },
                projectile_emitter = {
                    projectile_velocity = { x = 0, y = -0.5 },
                    projectile_duration = 2, -- seconds
                    cooldown = 1, -- seconds
                    hit_percentage_damage = 10,
                    is_projectile_friendly = false,
                },
            },
        },
        {
            -- Tank
            components = {
                enemy = {},
                transform = {
                    position = { x = 2852, y = 1520 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "tank-panther-right-texture",
                    width = 32,
                    height = 32,
                    z_index = 1,
                },
                box_collider = {
                    width = 22,
                    height = 18,
                    offset = { x = 5, y = 7 },
                },
                health = {
                    health_percentage = 100,
                },
                projectile_emitter = {
                    projectile_velocity = { x = 0.5, y = 0 },
                    projectile_duration = 2, -- seconds
                    cooldown = 1, -- seconds
                    hit_percentage_damage = 10,
                    is_projectile_friendly = false,
                },
            },
        },
        {
            -- Tank
            components = {
                enemy = {},
                transform = {
                    position = { x = 2846, y = 1670 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "tank-tiger-left-texture",
                    width = 32,
                    height = 32,
                    z_index = 1,
                },
                box_collider = {
                    width = 25,
                    height = 18,
                    offset = { x = 7, y = 7 },
                },
                health = {
                    health_percentage = 100,
                },
                projectile_emitter = {
                    projectile_velocity = { x = -0.5, y = 0 },
                    projectile_duration = 1, -- seconds
                    cooldown = 2, -- seconds
                    hit_percentage_damage = 10,
                    is_projectile_friendly = false,
                },
            },
        },
        {
            -- Tank
            components = {
                enemy = {},
                transform = {
                    position = { x = 2900, y = 600 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "tank-tiger-down-texture",
                    width = 32,
                    height = 32,
                    z_index = 1,
                },
                box_collider = {
                    width = 19,
                    height = 20,
                    offset = { x = 6, y = 7 },
                },
                health = {
                    health_percentage = 100,
                },
                projectile_emitter = {
                    projectile_velocity = { x = 0, y = 0.75 },
                    projectile_duration = 1, -- seconds
                    cooldown = 1, -- seconds
                    hit_percentage_damage = 10,
                    is_projectile_friendly = false,
                },
            },
        },
        {
            -- Tank
            components = {
                enemy = {},
                transform = {
                    position = { x = 390, y = 1960 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "tank-tiger-up-texture",
                    width = 32,
                    height = 32,
                    z_index = 1,
                },
                box_collider = {
                    width = 18,
                    height = 25,
                    offset = { x = 7, y = 7 },
                },
                health = {
                    health_percentage = 100,
                },
                projectile_emitter = {
                    projectile_velocity = { x = 0, y = -0.25 },
                    projectile_duration = 2, -- seconds
                    cooldown = 1, -- seconds
                    hit_percentage_damage = 10,
                    is_projectile_friendly = false,
                },
            },
        },
        {
            -- Tank
            components = {
                enemy = {},
                transform = {
                    position = { x = 220, y = 2250 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "tank-panther-down-texture",
                    width = 32,
                    height = 32,
                    z_index = 1,
                },
                box_collider = {
                    width = 17,
                    height = 20,
                    offset = { x = 8, y = 4 },
                },
                health = {
                    health_percentage = 100,
                },
                projectile_emitter = {
                    projectile_velocity = { x = 0, y = 0.75 },
                    projectile_duration = 1, -- seconds
                    cooldown = 1, -- seconds
                    hit_percentage_damage = 10,
                    is_projectile_friendly = false,
                },
            },
        },
        {
            -- Tank
            components = {
                enemy = {},
                transform = {
                    position = { x = 2790, y = 1080 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "tank-panther-killed-texture",
                    width = 32,
                    height = 32,
                    z_index = 1,
                },
            },
        },
        {
            -- Truck
            components = {
                enemy = {},
                transform = {
                    position = { x = 226, y = 1160 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "truck-ford-down-texture",
                    width = 32,
                    height = 32,
                    z_index = 1,
                },
                box_collider = {
                    width = 12,
                    height = 25,
                    offset = { x = 10, y = 2 },
                },
                health = {
                    health_percentage = 100,
                },
            },
        },
        {
            -- Truck
            components = {
                enemy = {},
                transform = {
                    position = { x = 360, y = 2090 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "truck-ford-down-texture",
                    width = 32,
                    height = 32,
                    z_index = 1,
                },
                box_collider = {
                    width = 12,
                    height = 25,
                    offset = { x = 10, y = 2 },
                },
                health = {
                    health_percentage = 100,
                },
            },
        },
        {
            -- Truck
            components = {
                enemy = {},
                transform = {
                    position = { x = 390, y = 2110 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "truck-ford-down-texture",
                    width = 32,
                    height = 32,
                    z_index = 1,
                },
                box_collider = {
                    width = 12,
                    height = 25,
                    offset = { x = 10, y = 2 },
                },
                health = {
                    health_percentage = 100,
                },
            },
        },
        {
            -- Truck
            components = {
                enemy = {},
                transform = {
                    position = { x = 420, y = 2130 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "truck-ford-down-texture",
                    width = 32,
                    height = 32,
                    z_index = 1,
                },
                box_collider = {
                    width = 12,
                    height = 25,
                    offset = { x = 10, y = 2 },
                },
                health = {
                    health_percentage = 100,
                },
            },
        },
        {
            -- Truck
            components = {
                enemy = {},
                transform = {
                    position = { x = 1090, y = 1320 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "truck-ford-down-texture",
                    width = 32,
                    height = 32,
                    z_index = 1,
                },
                box_collider = {
                    width = 12,
                    height = 25,
                    offset = { x = 10, y = 2 },
                },
                health = {
                    health_percentage = 100,
                },
            },
        },
        {
            -- Truck
            components = {
                enemy = {},
                transform = {
                    position = { x = 1120, y = 1340 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "truck-ford-down-texture",
                    width = 32,
                    height = 32,
                    z_index = 1,
                },
                box_collider = {
                    width = 12,
                    height = 25,
                    offset = { x = 10, y = 2 },
                },
                health = {
                    health_percentage = 100,
                },
            },
        },
        {
            -- Truck
            components = {
                enemy = {},
                transform = {
                    position = { x = 2720, y = 1760 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "truck-ford-up-texture",
                    width = 32,
                    height = 32,
                    z_index = 1,
                },
                box_collider = {
                    width = 12,
                    height = 20,
                    offset = { x = 10, y = 8 },
                },
                health = {
                    health_percentage = 100,
                },
            },
        },
        {
            -- Truck
            components = {
                enemy = {},
                transform = {
                    position = { x = 2760, y = 1760 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "truck-ford-up-texture",
                    width = 32,
                    height = 32,
                    z_index = 1,
                },
                box_collider = {
                    width = 12,
                    height = 20,
                    offset = { x = 10, y = 8 },
                },
                health = {
                    health_percentage = 100,
                },
            },
        },
        {
            -- Truck
            components = {
                enemy = {},
                transform = {
                    position = { x = 2800, y = 1760 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "truck-ford-up-texture",
                    width = 32,
                    height = 32,
                    z_index = 1,
                },
                box_collider = {
                    width = 12,
                    height = 20,
                    offset = { x = 10, y = 8 },
                },
                health = {
                    health_percentage = 100,
                },
            },
        },
        {
            -- Truck
            components = {
                enemy = {},
                transform = {
                    position = { x = 3010, y = 1560 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "truck-ford-left-texture",
                    width = 32,
                    height = 32,
                    z_index = 1,
                },
                box_collider = {
                    width = 25,
                    height = 16,
                    offset = { x = 3, y = 10 },
                },
                health = {
                    health_percentage = 100,
                },
            },
        },
        {
            -- Truck
            components = {
                enemy = {},
                transform = {
                    position = { x = 3030, y = 1580 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "truck-ford-left-texture",
                    width = 32,
                    height = 32,
                    z_index = 2,
                },
                box_collider = {
                    width = 25,
                    height = 16,
                    offset = { x = 3, y = 10 },
                },
                health = {
                    health_percentage = 100,
                },
            },
        },
        {
            -- SAM Tank
            components = {
                enemy = {},
                transform = {
                    position = { x = 990, y = 760 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "sam-tank-left-texture",
                    width = 32,
                    height = 32,
                    z_index = 2,
                },
                animation = {
                    frames_count = 2,
                    frames_per_second = 2, -- fps
                },
                box_collider = {
                    width = 17,
                    height = 15,
                    offset = { x = 8, y = 8 },
                },
                health = {
                    health_percentage = 100,
                },
                projectile_emitter = {
                    projectile_velocity = { x = -0.125, y = -0.125 },
                    projectile_duration = 3, -- seconds
                    cooldown = 2, -- seconds
                    hit_percentage_damage = 5,
                    is_projectile_friendly = false,
                },
            },
        },
        {
            -- SAM Tank
            components = {
                enemy = {},
                transform = {
                    position = { x = 990, y = 820 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "sam-tank-left-texture",
                    width = 32,
                    height = 32,
                    z_index = 2,
                },
                animation = {
                    frames_count = 2,
                    frames_per_second = 2, -- fps
                },
                box_collider = {
                    width = 17,
                    height = 15,
                    offset = { x = 8, y = 8 },
                },
                health = {
                    health_percentage = 100,
                },
                projectile_emitter = {
                    projectile_velocity = { x = -0.125, y = -0.125 },
                    projectile_duration = 3, -- seconds
                    cooldown = 2, -- seconds
                    hit_percentage_damage = 5,
                    is_projectile_friendly = false,
                },
            },
        },
        {
            -- SAM Tank
            components = {
                enemy = {},
                transform = {
                    position = { x = 2580, y = 230 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "sam-tank-left-texture",
                    width = 32,
                    height = 32,
                    z_index = 2,
                },
                animation = {
                    frames_count = 2,
                    frames_per_second = 2, -- fps
                },
                box_collider = {
                    width = 17,
                    height = 15,
                    offset = { x = 8, y = 8 },
                },
                health = {
                    health_percentage = 100,
                },
                projectile_emitter = {
                    projectile_velocity = { x = -0.25, y = -0.1875 },
                    projectile_duration = 2, -- seconds
                    cooldown = 2, -- seconds
                    hit_percentage_damage = 5,
                    is_projectile_friendly = false,
                },
            },
        },
        {
            -- SAM Tank
            components = {
                enemy = {},
                transform = {
                    position = { x = 1870, y = 1114 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "sam-tank-left-texture",
                    width = 32,
                    height = 32,
                    z_index = 2,
                },
                animation = {
                    frames_count = 2,
                    frames_per_second = 2, -- fps
                },
                box_collider = {
                    width = 17,
                    height = 15,
                    offset = { x = 8, y = 8 },
                },
                health = {
                    health_percentage = 100,
                },
                projectile_emitter = {
                    projectile_velocity = { x = -0.25, y = -0.15 },
                    projectile_duration = 2, -- seconds
                    cooldown = 2, -- seconds
                    hit_percentage_damage = 5,
                    is_projectile_friendly = false,
                },
            },
        },
        {
            -- SAM Truck
            components = {
                enemy = {},
                transform = {
                    position = { x = 228, y = 1400 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "sam-truck-right-texture",
                    width = 32,
                    height = 32,
                    z_index = 2,
                },
                box_collider = {
                    width = 17,
                    height = 15,
                    offset = { x = 8, y = 8 },
                },
                health = {
                    health_percentage = 100,
                },
                projectile_emitter = {
                    projectile_velocity = { x = 0.125, y = -0.125 },
                    projectile_duration = 2, -- seconds
                    cooldown = 1, -- seconds
                    hit_percentage_damage = 5,
                    is_projectile_friendly = false,
                },
            },
        },
        {
            -- SAM Truck
            components = {
                enemy = {},
                transform = {
                    position = { x = 228, y = 1440 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "sam-truck-right-texture",
                    width = 32,
                    height = 32,
                    z_index = 2,
                },
                box_collider = {
                    width = 17,
                    height = 15,
                    offset = { x = 8, y = 8 },
                },
                health = {
                    health_percentage = 100,
                },
                projectile_emitter = {
                    projectile_velocity = { x = 0.125, y = -0.125 },
                    projectile_duration = 2, -- seconds
                    cooldown = 1, -- seconds
                    hit_percentage_damage = 5,
                    is_projectile_friendly = false,
                },
            },
        },
        {
            -- SAM Truck
            components = {
                enemy = {},
                transform = {
                    position = { x = 232, y = 998 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "sam-truck-right-texture",
                    width = 32,
                    height = 32,
                    z_index = 2,
                },
                box_collider = {
                    width = 17,
                    height = 15,
                    offset = { x = 8, y = 8 },
                },
                health = {
                    health_percentage = 100,
                },
                projectile_emitter = {
                    projectile_velocity = { x = 0.125, y = -0.175 },
                    projectile_duration = 2, -- seconds
                    cooldown = 1, -- seconds
                    hit_percentage_damage = 5,
                    is_projectile_friendly = false,
                },
            },
        },
        {
            -- SAM Truck
            components = {
                enemy = {},
                transform = {
                    position = { x = 2908, y = 430 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "sam-truck-right-texture",
                    width = 32,
                    height = 32,
                    z_index = 2,
                },
                box_collider = {
                    width = 17,
                    height = 15,
                    offset = { x = 8, y = 8 },
                },
                health = {
                    health_percentage = 100,
                },
                projectile_emitter = {
                    projectile_velocity = { x = 0.125, y = -0.175 },
                    projectile_duration = 2, -- seconds
                    cooldown = 1, -- seconds
                    hit_percentage_damage = 5,
                    is_projectile_friendly = false,
                },
            },
        },
        {
            -- SAM Truck
            components = {
                enemy = {},
                transform = {
                    position = { x = 2908, y = 462 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "sam-truck-right-texture",
                    width = 32,
                    height = 32,
                    z_index = 2,
                },
                box_collider = {
                    width = 17,
                    height = 15,
                    offset = { x = 8, y = 8 },
                },
                health = {
                    health_percentage = 100,
                },
                projectile_emitter = {
                    projectile_velocity = { x = 0.125, y = -0.175 },
                    projectile_duration = 2, -- seconds
                    cooldown = 1, -- seconds
                    hit_percentage_damage = 5,
                    is_projectile_friendly = false,
                },
            },
        },
        {
            -- SAM Truck
            components = {
                enemy = {},
                transform = {
                    position = { x = 2908, y = 494 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "sam-truck-right-texture",
                    width = 32,
                    height = 32,
                    z_index = 2,
                },
                box_collider = {
                    width = 17,
                    height = 15,
                    offset = { x = 8, y = 8 },
                },
                health = {
                    health_percentage = 100,
                },
                projectile_emitter = {
                    projectile_velocity = { x = 0.125, y = -0.175 },
                    projectile_duration = 2, -- seconds
                    cooldown = 1, -- seconds
                    hit_percentage_damage = 5,
                    is_projectile_friendly = false,
                },
            },
        },
        {
            -- Vegetation
            components = {
                transform = {
                    position = { x = 230, y = 1266 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "tree5-texture",
                    width = 32,
                    height = 32,
                    z_index = 1,
                },
            },
        },
        {
            -- Vegetation
            components = {
                transform = {
                    position = { x = 234, y = 1300 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "tree5-texture",
                    width = 32,
                    height = 32,
                    z_index = 1,
                },
            },
        },
        {
            -- Vegetation
            components = {
                transform = {
                    position = { x = 636, y = 980 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "tree6-texture",
                    width = 32,
                    height = 32,
                    z_index = 1,
                },
            },
        },
        {
            -- Vegetation
            components = {
                transform = {
                    position = { x = 1336, y = 1052 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "tree14-texture",
                    width = 32,
                    height = 32,
                    z_index = 1,
                },
            },
        },
        {
            -- Vegetation
            components = {
                transform = {
                    position = { x = 2036, y = 1476 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "tree17-texture",
                    width = 17,
                    height = 20,
                    z_index = 1,
                },
            },
        },
        {
            -- Vegetation
            components = {
                transform = {
                    position = { x = 2068, y = 1476 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "tree17-texture",
                    width = 17,
                    height = 20,
                    z_index = 1,
                },
            },
        },
        {
            -- Vegetation
            components = {
                transform = {
                    position = { x = 2056, y = 1490 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "tree18-texture",
                    width = 17,
                    height = 20,
                    z_index = 2,
                },
            },
        },
        {
            -- Vegetation
            components = {
                transform = {
                    position = { x = 2780, y = 880 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "tree10-texture",
                    width = 31,
                    height = 32,
                    z_index = 1,
                },
            },
        },
        {
            -- Vegetation
            components = {
                transform = {
                    position = { x = 2800, y = 890 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "tree10-texture",
                    width = 31,
                    height = 32,
                    z_index = 2,
                },
            },
        },
        {
            -- Vegetation
            components = {
                transform = {
                    position = { x = 2730, y = 580 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "tree14-texture",
                    width = 32,
                    height = 32,
                    z_index = 2,
                },
            },
        },
        {
            -- Obstacles
            components = {
                transform = {
                    position = { x = 1338, y = 1098 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "obstacles7-texture",
                    width = 16,
                    height = 16,
                    z_index = 2,
                },
            },
        },
        {
            -- Obstacles
            components = {
                transform = {
                    position = { x = 1370, y = 1098 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "obstacles7-texture",
                    width = 16,
                    height = 16,
                    z_index = 2,
                },
            },
        },
        {
            -- Obstacles
            components = {
                transform = {
                    position = { x = 630, y = 1010 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "obstacles7-texture",
                    width = 16,
                    height = 16,
                    z_index = 3,
                },
            },
        },
        {
            -- Obstacles
            components = {
                transform = {
                    position = { x = 660, y = 1014 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "obstacles7-texture",
                    width = 16,
                    height = 16,
                    z_index = 2,
                },
            },
        },
        {
            -- Obstacles
            components = {
                transform = {
                    position = { x = 876, y = 780 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "obstacles7-texture",
                    width = 16,
                    height = 16,
                    z_index = 1,
                },
            },
        },
        {
            -- Obstacles
            components = {
                transform = {
                    position = { x = 898, y = 816 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "obstacles7-texture",
                    width = 16,
                    height = 16,
                    z_index = 2,
                },
            },
        },
        {
            -- Obstacles
            components = {
                transform = {
                    position = { x = 862, y = 832 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "obstacles7-texture",
                    width = 16,
                    height = 16,
                    z_index = 2,
                },
            },
        },
        {
            -- Obstacles
            components = {
                transform = {
                    position = { x = 1880, y = 1390 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "obstacles7-texture",
                    width = 16,
                    height = 16,
                    z_index = 2,
                },
            },
        },
        {
            -- Obstacles
            components = {
                transform = {
                    position = { x = 1910, y = 1410 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "obstacles7-texture",
                    width = 16,
                    height = 16,
                    z_index = 2,
                },
            },
        },
        {
            -- Obstacles
            components = {
                transform = {
                    position = { x = 2170, y = 1014 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "obstacles7-texture",
                    width = 16,
                    height = 16,
                    z_index = 2,
                },
            },
        },
        {
            -- Obstacles
            components = {
                transform = {
                    position = { x = 2150, y = 1054 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "obstacles7-texture",
                    width = 16,
                    height = 16,
                    z_index = 2,
                },
            },
        },
        {
            -- Obstacles
            components = {
                transform = {
                    position = { x = 2150, y = 1094 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "obstacles7-texture",
                    width = 16,
                    height = 16,
                    z_index = 2,
                },
            },
        },
        {
            -- Obstacles
            components = {
                transform = {
                    position = { x = 2170, y = 1134 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "obstacles7-texture",
                    width = 16,
                    height = 16,
                    z_index = 2,
                },
            },
        },
        {
            -- Obstacles
            components = {
                transform = {
                    position = { x = 2710, y = 898 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "obstacles2-texture",
                    width = 32,
                    height = 32,
                    z_index = 1,
                },
            },
        },
        {
            -- Obstacles
            components = {
                transform = {
                    position = { x = 2860, y = 892 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "obstacles2-texture",
                    width = 32,
                    height = 32,
                    z_index = 1,
                },
            },
        },
        {
            -- Obstacles
            components = {
                transform = {
                    position = { x = 2870, y = 390 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "obstacles7-texture",
                    width = 16,
                    height = 16,
                    z_index = 2,
                },
            },
        },
        {
            -- Obstacles
            components = {
                transform = {
                    position = { x = 2850, y = 430 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "obstacles7-texture",
                    width = 16,
                    height = 16,
                    z_index = 2,
                },
            },
        },
        {
            -- Obstacles
            components = {
                transform = {
                    position = { x = 2850, y = 470 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "obstacles7-texture",
                    width = 16,
                    height = 16,
                    z_index = 2,
                },
            },
        },
        {
            -- Obstacles
            components = {
                transform = {
                    position = { x = 2850, y = 510 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "obstacles7-texture",
                    width = 16,
                    height = 16,
                    z_index = 2,
                },
            },
        },
        {
            -- Obstacles
            components = {
                transform = {
                    position = { x = 2870, y = 550 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "obstacles7-texture",
                    width = 16,
                    height = 16,
                    z_index = 2,
                },
            },
        },
        {
            -- Obstacles
            components = {
                transform = {
                    position = { x = 2720, y = 620 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "obstacles7-texture",
                    width = 16,
                    height = 16,
                    z_index = 1,
                },
            },
        },
        {
            -- Obstacles
            components = {
                transform = {
                    position = { x = 2760, y = 624 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "obstacles7-texture",
                    width = 16,
                    height = 16,
                    z_index = 1,
                },
            },
        },
        {
            -- Obstacles
            components = {
                transform = {
                    position = { x = 2660, y = 424 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "obstacles1-texture",
                    width = 32,
                    height = 32,
                    z_index = 1,
                },
            },
        },
        {
            -- Obstacles
            components = {
                transform = {
                    position = { x = 2720, y = 464 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "obstacles1-texture",
                    width = 32,
                    height = 32,
                    z_index = 1,
                },
            },
        },
        {
            -- Army
            components = {
                transform = {
                    position = { x = 1260, y = 810 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "army-walk-left-texture",
                    width = 32,
                    height = 32,
                    z_index = 1,
                },
            },
        },
        {
            -- Army
            components = {
                transform = {
                    position = { x = 994, y = 900 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "army-walk-right-texture",
                    width = 32,
                    height = 32,
                    z_index = 1,
                },
            },
        },
        {
            -- Army
            components = {
                transform = {
                    position = { x = 1144, y = 1120 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "army-gun-left-texture",
                    width = 32,
                    height = 32,
                    z_index = 1,
                },
            },
        },
        {
            -- Army
            components = {
                transform = {
                    position = { x = 228, y = 1960 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "army-walk-up-texture",
                    width = 32,
                    height = 32,
                    z_index = 1,
                },
            },
        },
        {
            -- Army
            components = {
                transform = {
                    position = { x = 228, y = 2060 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "army-walk-down-texture",
                    width = 32,
                    height = 32,
                    z_index = 1,
                },
            },
        },
        {
            -- Army
            components = {
                transform = {
                    position = { x = 1766, y = 980 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "army-walk-left-texture",
                    width = 32,
                    height = 32,
                    z_index = 1,
                },
            },
        },
        {
            -- Army
            components = {
                transform = {
                    position = { x = 226, y = 1702 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "army-gun-right-texture",
                    width = 32,
                    height = 32,
                    z_index = 1,
                },
            },
        },
        {
            -- Army
            components = {
                transform = {
                    position = { x = 1500, y = 1260 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "army-walk-left-texture",
                    width = 32,
                    height = 32,
                    z_index = 1,
                },
            },
        },
        {
            -- Army
            components = {
                transform = {
                    position = { x = 1600, y = 1260 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "army-walk-right-texture",
                    width = 32,
                    height = 32,
                    z_index = 1,
                },
            },
        },
        {
            -- Army
            components = {
                transform = {
                    position = { x = 1712, y = 230 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "army-gun-down-texture",
                    width = 32,
                    height = 32,
                    z_index = 1,
                },
            },
        },
        {
            -- Army
            components = {
                transform = {
                    position = { x = 1800, y = 230 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "army-gun-up-texture",
                    width = 32,
                    height = 32,
                    z_index = 1,
                },
            },
        },
        {
            -- Army
            components = {
                transform = {
                    position = { x = 2234, y = 1060 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "army-walk-right-texture",
                    width = 32,
                    height = 32,
                    z_index = 1,
                },
            },
        },
        {
            -- Army
            components = {
                transform = {
                    position = { x = 1510, y = 880 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "army-gun-down-texture",
                    width = 32,
                    height = 32,
                    z_index = 1,
                },
            },
        },
        {
            -- Army
            components = {
                transform = {
                    position = { x = 1620, y = 880 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "army-gun-down-texture",
                    width = 32,
                    height = 32,
                    z_index = 1,
                },
            },
        },
        {
            -- Army
            components = {
                transform = {
                    position = { x = 2780, y = 1380 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "army-walk-killed-texture",
                    width = 32,
                    height = 32,
                    z_index = 1,
                },
            },
        },
        {
            -- Army
            components = {
                transform = {
                    position = { x = 2850, y = 1380 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "army-walk-killed-texture",
                    width = 32,
                    height = 32,
                    z_index = 1,
                },
            },
        },
        {
            -- Army
            components = {
                transform = {
                    position = { x = 2930, y = 1380 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "army-walk-killed-texture",
                    width = 32,
                    height = 32,
                    z_index = 1,
                },
            },
        },
        {
            -- Boat
            components = {
                transform = {
                    position = { x = 160, y = 1040 },
                    scale = { x = 1.6, y = 1.6 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "boat-texture",
                    width = 21,
                    height = 126,
                    z_index = 1,
                },
            },
        },
        {
            -- Boat
            components = {
                transform = {
                    position = { x = 160, y = 1580 },
                    scale = { x = 1.6, y = 1.6 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "boat-texture",
                    width = 21,
                    height = 126,
                    z_index = 1,
                },
            },
        },
        {
            -- Boat
            components = {
                transform = {
                    position = { x = 690, y = 846 },
                    scale = { x = 1.6, y = 1.6 },
                    rotation = 270.0, -- degrees
                },
                sprite = {
                    texture_id = "boat-texture",
                    width = 21,
                    height = 126,
                    z_index = 1,
                },
            },
        },
        {
            -- Boat
            components = {
                transform = {
                    position = { x = 3020, y = 920 },
                    scale = { x = 1.6, y = 1.6 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "boat-texture",
                    width = 21,
                    height = 126,
                    z_index = 1,
                },
            },
        },
        {
            -- Carrier
            components = {
                transform = {
                    position = { x = 1340, y = 300 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "carrier-texture",
                    width = 59,
                    height = 191,
                    z_index = 1,
                },
            },
        },
        {
            -- Carrier
            components = {
                transform = {
                    position = { x = 600, y = 1950 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "carrier-texture",
                    width = 59,
                    height = 191,
                    z_index = 1,
                },
            },
        },
        {
            -- F-22 fighter jet
            components = {
                enemy = {},
                transform = {
                    position = { x = 1376, y = 330 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "f22-texture",
                    width = 32,
                    height = 32,
                    z_index = 4,
                },
                animation = {
                    frames_count = 2,
                    frames_per_second = 10, -- fps
                },
                box_collider = {
                    width = 20,
                    height = 25,
                    offset = { x = 5, y = 5 },
                },
                health = {
                    health_percentage = 100,
                },
            },
        },
        {
            -- SU-27 fighter jet
            components = {
                enemy = {},
                transform = {
                    position = { x = 1370, y = 600 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = -10.0, -- degrees
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
                    frames_per_second = 10, -- fps
                },
                box_collider = {
                    width = 25,
                    height = 30,
                    offset = { x = 5, y = 0 },
                },
                health = {
                    health_percentage = 100,
                },
            },
        },
        {
            -- Bomber plane
            components = {
                enemy = {},
                transform = {
                    position = { x = 928, y = 1040 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "bomber-texture",
                    width = 32,
                    height = 32,
                    z_index = 5,
                },
                animation = {
                    frames_count = 2,
                    frames_per_second = 10, -- fps
                },
                box_collider = {
                    width = 32,
                    height = 32,
                    offset = { x = 0, y = 0 },
                },
                health = {
                    health_percentage = 100,
                },
            },
        },
        {
            -- FW190 plane
            components = {
                enemy = {},
                transform = {
                    position = { x = 2000, y = 286 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = -90.0, -- degrees
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
                    frames_per_second = 15, -- fps
                },
                box_collider = {
                    width = 32,
                    height = 30,
                    offset = { x = 0, y = 0 },
                },
                health = {
                    health_percentage = 100,
                },
            },
        },
        {
            -- UPF7 plane
            components = {
                enemy = {},
                transform = {
                    position = { x = 1840, y = 358 },
                    scale = { x = 1.6, y = 1.6 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "upf7-texture",
                    width = 32,
                    height = 32,
                    z_index = 5,
                },
                animation = {
                    frames_count = 2,
                    frames_per_second = 10, -- fps
                },
            },
        },
        {
            -- UPF7 plane
            components = {
                enemy = {},
                transform = {
                    position = { x = 1900, y = 358 },
                    scale = { x = 1.6, y = 1.6 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "upf7-texture",
                    width = 32,
                    height = 32,
                    z_index = 5,
                },
                animation = {
                    frames_count = 2,
                    frames_per_second = 10, -- fps
                },
            },
        },
        {
            -- UPF7 plane
            components = {
                enemy = {},
                transform = {
                    position = { x = 1960, y = 358 },
                    scale = { x = 1.6, y = 1.6 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_id = "upf7-texture",
                    width = 32,
                    height = 32,
                    z_index = 5,
                },
                animation = {
                    frames_count = 2,
                    frames_per_second = 10, -- fps
                },
            },
        },
        {
            -- SU-27 fighter jet
            components = {
                enemy = {},
                transform = {
                    position = { x = 634, y = 1970 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                rigid_body = {
                    velocity = { x = 0.0, y = -0.125 },
                },
                sprite = {
                    texture_id = "su27-texture",
                    width = 32,
                    height = 32,
                    z_index = 5,
                },
                animation = {
                    frames_count = 2,
                    frames_per_second = 10, -- fps
                },
                box_collider = {
                    width = 32,
                    height = 32,
                },
                health = {
                    health_percentage = 100,
                },
                projectile_emitter = {
                    projectile_velocity = { x = 0, y = -0.25 },
                    projectile_duration = 5, -- seconds
                    cooldown = 1, -- seconds
                    hit_percentage_damage = 10,
                    is_projectile_friendly = false,
                },
                script = {
                    on_update = function(entity, delta_time, elapsed_time)
                        -- this function makes the fighter jet move up and down the map shooting projectiles
                        local current_position = get_position(entity)
                        local current_velocity = get_velocity(entity)

                        -- if it reaches the top or the bottom of the map
                        if current_position.y < 10 or current_position.y > map_height - 32 then
                            set_velocity(entity, Vec2.new(0, current_velocity.y * -1)) -- flip the entity y-velocity
                        else
                            set_velocity(entity, Vec2.new(0, current_velocity.y)) -- do not flip y-velocity
                        end

                        -- set the transform rotation to match going up or down
                        if current_velocity.y < 0 then
                            set_rotation(entity, 0) -- point up
                            set_projectile_velocity(entity, Vec2.new(0, -0.5)) -- shoot projectiles up
                        else
                            set_rotation(entity, 180) -- point down
                            set_projectile_velocity(entity, Vec2.new(0, 0.5)) -- shoot projectiles down
                        end
                    end,
                },
            },
        },
        {
            -- F-22 fighter jet
            components = {
                enemy = {},
                transform = {
                    position = { x = 20, y = 20 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 90.0, -- degrees
                },
                rigid_body = {
                    velocity = { x = 0.0, y = 0.0 },
                },
                sprite = {
                    texture_id = "f22-texture",
                    width = 32,
                    height = 32,
                    z_index = 5,
                },
                animation = {
                    frames_count = 2,
                    frames_per_second = 10, -- fps
                },
                box_collider = {
                    width = 32,
                    height = 32,
                },
                health = {
                    health_percentage = 100,
                },
                projectile_emitter = {
                    projectile_velocity = { x = 0.5, y = 0 },
                    projectile_duration = 1, -- secondsm
                    cooldown = 1, -- seconds
                    hit_percentage_damage = 10,
                    is_projectile_friendly = false,
                },
                script = {
                    on_update = function(entity, delta_time, elapsed_time)
                        -- change the position of the the airplane to follow a sine wave movement
                        local new_x = elapsed_time * 0.09
                        local new_y = 200 + (math.sin(elapsed_time * 0.001) * 50)
                        set_position(entity, Vec2.new(new_x, new_y)) -- set the new position
                    end,
                },
            },
        },
    },
}

map_width = Level.tilemap.map_cols * Level.tilemap.tile_size * Level.tilemap.scale
map_height = Level.tilemap.map_rows * Level.tilemap.tile_size * Level.tilemap.scale
