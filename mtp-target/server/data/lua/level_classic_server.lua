---------------------- Level  ----------------------



---------------------- Entity ----------------------
local clientId = 0;

function Entity:init()
  --print(self:getName());
  --print("entity init");
end

function Entity:update()
  --print(self:getName());
  --print("entity update");
end


function entitySceneCollideEvent ( entity, module )
  --print(entity:getName());
  --print(module:getName());
  --print("entitySceneCollideEvent");
  --print(entity:getName());
  --print(module:getScore());
  
  module:collide(entity);
  --entity:setCurrentScore(module:getScore());
end

function entityEntityCollideEvent ( entity1, entity2 )
  --print(entity1:getName());
  --print(entity2:getName());
  --print("entityEntityCollideEvent");
end

function entityWaterCollideEvent ( entity )
  --print(entity:getName());
  --print("entityWaterCollideEvent");
end

function Module:collide( entity )
  if(entity:getIsOpen()==0) then
    entity:setCurrentScore(self:getScore());
   end
end




