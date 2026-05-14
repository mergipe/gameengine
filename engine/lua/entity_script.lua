EntityScript = { entity = {} }

function EntityScript:new(o)
  o = o or {}
  self.__index = self
  setmetatable(o, self)
  return o
end

function EntityScript:OnStart() end

function EntityScript:OnUpdate() end

function EntityScript:GetComponent(componentType)
  return ApiGetComponent(self.entity, componentType)
end
