KillOnTouch = EntityScript:new()

function KillOnTouch:OnCollisionEnter(otherCollider, collisionData)
  otherCollider.entity:Destroy()
end
