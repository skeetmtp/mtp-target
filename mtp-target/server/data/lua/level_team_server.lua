---------------------- Level  ----------------------



---------------------- Entity ----------------------
CEntity = {}
CEntity_mt = {}
function CEntity:new()
  return setmetatable({ team = 0, id = 0 }, CEntity_mt)
end

function CEntity:print()
  table.foreach(self,print);
end

CEntity_mt.__index = CEntity

local clientId = 0;

function CEntity:getTeam()
  return self.team;
end

function CEntity:setTeam( t )
  self.team = t;
end

function Entity:parent()
  return self:getUserData();
end

function Entity:init()
  local parent = CEntity:new();
  self:setUserData(parent);
  clientId = clientId + 1;
  local t = math.mod(clientId,2);

  parent:setTeam(t);
  print(self:getName());
  print(parent:getTeam());
end

function Entity:update()
end


function entitySceneCollideEvent ( entity, module )
  module:collide(entity);
end

function entityEntityCollideEvent ( entity1, entity2 )
end

function entityWaterCollideEvent ( entity )
end

function Module:collide( entity )
  print(entity:getName());
  print(entity:parent():getTeam());
  --entity:setCurrentScore(self:getScore());
end




