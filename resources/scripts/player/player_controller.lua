require("entity_script")

PlayerController = EntityScript:new()

function PlayerController:on_start()
  self.rotation_speed = 0.02
  self.rotation_value = 0
  self.acceleration = 0.01
  self.acceleration_value = 0
  self.is_shooting = false
  self.transform = self:get_component_transform()
  self.rigidbody = self:get_component_rigidbody2d()
end

function PlayerController:on_update(time_step)
  self.transform.rotation.z = self.transform.rotation.z - self.rotation_value * self.rotation_speed
end

function PlayerController:on_rotate_left(input_value)
  self.rotation_value = -input_value.value
end

function PlayerController:on_rotate_right(input_value)
  self.rotation_value = input_value.value
end

function PlayerController:on_accelerate(input_value)
  self.acceleration_value = input_value.value
end

function PlayerController:on_shoot(input_value)
  self.is_shooting = (input_value.value ~= 0)
end
