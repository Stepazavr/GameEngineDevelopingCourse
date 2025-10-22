local ecs = require "ecs"


local function UpdateTimer(it)
    for timer, ent in ecs.each(it) do
      if timer.isActive == 1.0 then
        timer.value = timer.value - it.delta_time

        if timer.value <= 0.0 then
          timer.isActive = 0.0
        end

      end
    end 
end


local function DeleteEntity(it)
    for pos, alive, ent in ecs.each(it) do
      if alive.isAlive == 0.0 then
        pos.x = 1000.0
        pos.y = 6.0
        pos.z = 1000.0
      end
    end
end


local function UpdateEntityWithTimer(it)
    for timer, alive, ent in ecs.each(it) do
      if timer.value <= 0.0 then
        alive.isAlive = 0.0
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
    local entities = {}
    
    for pos, vel, alive, ent in ecs.each(it) do
        table.insert(entities, ent)
    end

    local Position = ecs.lookup("Position")
    local Velocity = ecs.lookup("Velocity") 
    local Alive = ecs.lookup("Alive")
    
    for i = 1, #entities do
        local e1 = entities[i]
        local pos1 = ecs.get(e1, Position)
        local vel1 = ecs.get(e1, Velocity)
        local alive1 = ecs.get(e1, Alive)

        if alive1.isAlive == 1.0 then
            
            for j = i + 1, #entities do
                local e2 = entities[j]
                local pos2 = ecs.get(e2, Position)
                local vel2 = ecs.get(e2, Velocity)
                local alive2 = ecs.get(e2, Alive)

                if alive2.isAlive == 1.0 and distance(pos1, pos2) < 1.5 then
                    ecs.set(e1, Velocity, { x = vel2.x, y = vel2.y + 2.0, z = vel2.z })
                    ecs.set(e2, Velocity, { x = vel1.x,  y = vel1.y + 2.0, z = vel1.z })
                end
            end
        end
    end
end



ecs.system(UpdateTimer, "UpdateTimer", ecs.OnUpdate, "TriggerTimer")
ecs.system(DeleteEntity, "DeleteEntity", ecs.OnUpdate, "Position, Alive")
ecs.system(UpdateEntityWithTimer, "UpdateEntityWithTimer", ecs.OnUpdate, "TriggerTimer, Alive")
ecs.system(Collision, "Collision", ecs.OnUpdate, "Position, Velocity, Alive")
