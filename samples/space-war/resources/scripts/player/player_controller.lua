require("entity_script")

PlayerController = EntityScript:new()

function PlayerController:on_start()
  self.rotation_speed = 10000000000
  self.rotation_value = 0
  self.acceleration_magnitude = 10000000
  self.is_accelerating = false
  self.is_shooting = false
  self.transform = self:get_component_transform()
  self.rigidbody = self:get_component_rigidbody2d()
end

function PlayerController:on_update(time_step)
  if self.is_accelerating then
    local up = self.transform.up
    self.rigidbody.body:apply_force_to_center(
      Vec2.new(-up.x * self.acceleration_magnitude, -up.y * self.acceleration_magnitude)
    )
  end
  if self.rotation_value ~= 0 then
    self.rigidbody.body:apply_torque(-self.rotation_value * self.rotation_speed)
  end
end

function PlayerController:on_rotate_left(input_value)
  self.rotation_value = -input_value.value
end

function PlayerController:on_rotate_right(input_value)
  self.rotation_value = input_value.value
end

function PlayerController:on_accelerate(input_value)
  if input_value.value == 0 then
    self.is_accelerating = false
  else
    self.is_accelerating = true
  end
end

function PlayerController:on_shoot(input_value)
  self.is_shooting = (input_value.value ~= 0)
end
