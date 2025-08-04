EntityScript = { entity = {} }

function EntityScript:new(o)
  o = o or {}
  self.__index = self
  setmetatable(o, self)
  return o
end

function EntityScript:on_start() end

function EntityScript:on_update() end

function EntityScript:get_component_id()
  return api_get_component_id(self.entity)
end

function EntityScript:get_component_tag()
  return api_get_component_tag(self.entity)
end

function EntityScript:get_component_transform()
  return api_get_component_transform(self.entity)
end

function EntityScript:get_component_rigidbody2d()
  return api_get_component_rigidbody2d(self.entity)
end

function EntityScript:get_component_sprite()
  return api_get_component_sprite(self.entity)
end

function EntityScript:get_component_spriteanimation()
  return api_get_component_spriteanimation(self.entity)
end

function EntityScript:get_component_boxcollider2d()
  return api_get_component_boxcollider2d(self.entity)
end

function EntityScript:get_component_playerinput()
  return api_get_component_playerinput(self.entity)
end

function EntityScript:get_component_script()
  return api_get_component_script(self.entity)
end

function EntityScript:get_component_camera()
  return api_get_component_camera(self.entity)
end
