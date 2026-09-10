SelfDestroyOnTouch = EntityScript:new()

function SelfDestroyOnTouch:OnCollisionEnter()
  self.entity:Destroy()
end
