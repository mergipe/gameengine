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
    -- {
    --     type = "texture",
    --     id = "tilemap-texture",
    --     file = "tilemaps/jungle.png",
    --     min_filter = "nearest",
    --     mag_filter = "nearest",
    -- },
  },
  -- tilemap = {
  --     map_file = "resources/tilemaps/jungle.map",
  --     texture_id = "tilemap-texture",
  --     map_rows = 20,
  --     map_cols = 25,
  --     tileset_cols = 10,
  --     tile_size = 32,
  --     scale = 6.0,
  -- },
  entities = {
    {
      components = {
        tag = "camera",
        transform = {
          position = { x = 0, y = 0, z = 3 },
        },
        camera = {
          projection = "orthographic",
          z_near = 0.1,
          z_far = 100,
          -- fov_y = 45
        },
      },
    },
    {
      components = {
        tag = "player",
        transform = {
          position = { x = -1500, y = 800 },
          scale = { x = 0.7, y = 0.7 },
        },
        rigid_body = {
          velocity = { x = 0.0, y = 0.0 },
        },
        sprite = {
          texture_id = "spritesheet-texture",
          texture_x = 896,
          texture_y = 305,
          width = 228,
          height = 163,
          color = { r = 1.0, g = 1.0, b = 1.0 },
          z_index = 4,
        },
        -- animation = {
        --     frames_count = 2,
        --     frames_per_second = 10,
        -- },
        box_collider = {
          width = 228,
          height = 163,
          offset = { x = 0, y = 0 },
        },
        player_input = {},
      },
    },
    {
      components = {
        tag = "player",
        transform = {
          position = { x = 1500, y = 800 },
          scale = { x = 0.7, y = 0.7 },
        },
        rigid_body = {
          velocity = { x = 0.0, y = 0.0 },
        },
        sprite = {
          texture_id = "spritesheet-texture",
          texture_x = 896,
          texture_y = 0,
          width = 252,
          height = 216,
          color = { r = 1.0, g = 1.0, b = 1.0 },
          z_index = 4,
        },
        -- animation = {
        --     frames_count = 2,
        --     frames_per_second = 10,
        -- },
        box_collider = {
          width = 252,
          height = 216,
          offset = { x = 0, y = 0 },
        },
        player_input = {},
      },
    },
    {
      components = {
        tag = "planet",
        transform = {
          position = { x = 0, y = 0 },
          scale = { x = 1.0, y = 1.0 },
        },
        sprite = {
          texture_id = "planet-texture",
          width = 1280,
          height = 1280,
          z_index = 1,
          texture_x = 0,
          texture_y = 0,
        },
        box_collider = {
          width = 1030,
          height = 1030,
        },
      },
    },
  },
}
