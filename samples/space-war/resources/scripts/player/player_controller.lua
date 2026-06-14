PlayerController = EntityScript:new()

PlayerController.rotationSpeed = 300000000
PlayerController.accelerationMagnitude = 2000000

function PlayerController:OnStart()
  self.rotationValue = 0
  self.isAccelerating = false
  self.isShooting = false
  self.shotCooldown = 100
  self.lastShotTime = Timer.GetMilliseconds()
  self.transform = self.entity:GetComponent(Transform)
  self.rigidBody = self.entity:GetComponent(RigidBody2D)
end

function PlayerController:OnUpdate()
  if self.isAccelerating then
    local up = self.transform.up
    self.rigidBody:ApplyForce(Vec2.new(-up.x * self.accelerationMagnitude, -up.y * self.accelerationMagnitude))
  end
  if self.rotationValue ~= 0 then
    self.rigidBody:ApplyTorque(-self.rotationValue * self.rotationSpeed)
  end
  if self.isShooting then
    local currentTime = Timer.GetMilliseconds()
    if currentTime - self.lastShotTime > self.shotCooldown then
      local missile = Entity.Create(StringId.new("templates/missile.template"))
      local transform = missile:GetComponent(Transform)
      transform.position = self.transform.position - self.transform.up * 50
      transform.rotation = Vec3:new(0, 0, self.transform.rotation.z + math.pi)
      local rigidBody = missile:GetComponent(RigidBody2D)
      rigidBody.linearVelocity = -self.transform.up * 500
      self.lastShotTime = currentTime
    end
  end
end

function PlayerController:OnCollisionEnter(otherCollider, collisionData)
  if otherCollider.entity.tag.str == "planet" then
    self.entity:Destroy()
  else
    otherCollider.entity:Destroy()
  end
end

function PlayerController:OnCollisionExit(otherCollider) end

function PlayerController:OnTriggerEnter(otherCollider) end

function PlayerController:OnTriggerExit(otherCollider) end

function PlayerController:OnRotateLeft(inputValue)
  self.rotationValue = -inputValue.value
end

function PlayerController:OnRotateRight(inputValue)
  self.rotationValue = inputValue.value
end

function PlayerController:OnAccelerate(inputValue)
  if inputValue.value == 0 then
    self.isAccelerating = false
  else
    self.isAccelerating = true
  end
end

function PlayerController:OnShoot(inputValue)
  self.isShooting = (inputValue.value ~= 0)
end
