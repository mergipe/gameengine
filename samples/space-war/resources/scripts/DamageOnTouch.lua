DamageOnTouch = EntityScript:new()

DamageOnTouch.amount = 10

function DamageOnTouch:OnCollisionEnter(otherCollider, collisionData)
  local otherHealth = otherCollider.entity:GetComponent(Health)
  if otherHealth ~= nil then
    otherHealth.value = otherHealth.value - self.amount
    if otherHealth.value <= 0 then
      otherCollider.entity:Destroy()
    end
  end
end
