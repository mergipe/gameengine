local M = {}

function M.instance_of(object, type)
  type = tostring(type)
  local mt = getmetatable(object)
  while true do
    if mt == nil then
      return false
    end
    if tostring(mt) == type then
      return true
    end
    mt = getmetatable(mt)
  end
end

return M
