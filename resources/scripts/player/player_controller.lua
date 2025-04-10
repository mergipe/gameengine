require("entity_script")

PlayerController = EntityScript:new()

function PlayerController:on_start()
  self.transform = self:get_component_transform()
end

function PlayerController:on_update(time_step)
  self.transform.rotation.z = self.transform.rotation.z + 0.01
end
