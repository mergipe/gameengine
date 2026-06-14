EntityScript = { entity = {} }

function EntityScript:new(o)
  o = o or {}
  self.__index = self
  setmetatable(o, self)
  return o
end

function EntityScript:OnStart() end

function EntityScript:OnUpdate(timeStep) end

function EntityScript:OnLateUpdate(timeStep) end

function EntityScript:OnCollisionEnter(otherCollider, collisionData) end

function EntityScript:OnCollisionExit(otherCollider) end

function EntityScript:OnTriggerEnter(otherCollider) end

function EntityScript:OnTriggerExit(otherCollider) end
