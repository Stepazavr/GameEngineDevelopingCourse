local ecs = require "ecs"


local function UpdateTimer(it)
    for timer, ent in ecs.each(it) do
      if timer.isActive then
        timer.value = timer.value - it.delta_time

        if timer.value <= 0.0 then
          timer.isActive = false
        end

      end
    end 
end


local function DeleteEntity(it)
    for pos, alive, ent in ecs.each(it) do
      if not alive.isAlive then
        pos.x = 1000.0
        pos.y = 0.0
        pos.z = 1000.0
      end
    end
end



local function UpdateEntityWithTimer(it)
    for timer, alive, ent in ecs.each(it) do

      if not timer.isActive then
        timer.isActive = true
      end

      if timer.value <= 0.0 then
        alive.isAlive = false
      end
    end 
end


local function distance(pos1, pos2)
    local dx = pos1.x - pos2.x
    local dy = pos1.y - pos2.y
    local dz = pos1.z - pos2.z

    return math.sqrt(dx * dx + dy * dy + dz * dz)
end


local function Collision(it)
    for pos, targetComp, ent in ecs.each(it) do
      local targetId = targetComp.target

      if ecs.is_alive(targetId) then

        local aliveComp = ecs.get(targetId, ecs.lookup("Alive"))

        if aliveComp and aliveComp.isAlive then

          local targetPos = ecs.get(targetId, ecs.lookup("Position"))

          if distance(pos, targetPos) < 1.5 then   
            ecs.set(targetId, ecs.lookup("Velocity"), { x = 0.0, y = 20.0, z = 0.0 })
          end
        end
      end
    end 
end


ecs.system(UpdateTimer, "UpdateTimer", ecs.OnUpdate, "TriggerTimer")
ecs.system(DeleteEntity, "DeleteEntity", ecs.OnUpdate, "Position, Alive")
ecs.system(UpdateEntityWithTimer, "UpdateEntityWithTimer", ecs.OnUpdate, "TriggerTimer, Alive")
ecs.system(Collision, "Collision", ecs.OnUpdate, "Position, TargetEntity")
