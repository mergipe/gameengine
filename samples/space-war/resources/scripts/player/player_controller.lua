require("entity_script")

PlayerController = EntityScript:new()

function PlayerController:OnStart()
  self.rotation_speed = 10000000000
  self.rotation_value = 0
  self.acceleration_magnitude = 10000000
  self.is_accelerating = false
  self.is_shooting = false
  self.transform = self:GetComponent(Transform)
  self.rigidbody = self:GetComponent(RigidBody2D)
end

function PlayerController:OnUpdate(time_step)
  if self.is_accelerating then
    local up = self.transform.up
    self.rigidbody:ApplyForceToCenter(
      Vec2.new(-up.x * self.acceleration_magnitude, -up.y * self.acceleration_magnitude)
    )
  end
  if self.rotation_value ~= 0 then
    self.rigidbody:ApplyTorque(-self.rotation_value * self.rotation_speed)
  end
end

function PlayerController:OnRotateLeft(input_value)
  self.rotation_value = -input_value.value
end

function PlayerController:OnRotateRight(input_value)
  self.rotation_value = input_value.value
end

function PlayerController:OnAccelerate(input_value)
  if input_value.value == 0 then
    self.is_accelerating = false
  else
    self.is_accelerating = true
  end
end

function PlayerController:OnShoot(input_value)
  self.is_shooting = (input_value.value ~= 0)
end
