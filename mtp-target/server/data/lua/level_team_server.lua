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

function CEntity:printTeam()
  if(self.team==0) then
	print("red");
  else
	print("blue");
  end;
end

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
  self:setStartPointId(clientId+getSessionId()*2);
  local t = math.mod(clientId,2);

  if(t==0) then
  	self:displayText(0,5,1,255,0,0,"You are in RED team",15);
  	self:displayText(0,6,1,255,0,0,"Land on RED target !",20);
  else
  	self:displayText(0,5,1,100,100,255,"You are in BLUE team",15);
  	self:displayText(0,6,1,100,100,255,"Land on BLUE target !",20);
  end

  parent:setTeam(t);
  print(self:getName());
  parent:printTeam();
  clientId = clientId + 1;
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

---------------------- Module ----------------------
CModule = {}
CModule_mt = {}
function CModule:new(t)
  return setmetatable({ team = t or 0, id = 0 }, CModule_mt)
end

function CModule:getTeam()
  return self.team;
end

function CModule:setTeam( t )
  self.team = t;
end


CModule_mt.__index = CModule



function Module:parent()
  return self:getUserData();
end


function Module:collide( entity )
  if(entity:getIsOpen()==1) then
    entity:setCurrentScore(0)
  else
    if(self:parent():getTeam()==entity:parent():getTeam()) then
      entity:setCurrentScore(self:getScore())
    else
      entity:setCurrentScore(-self:getScore())
    end
  end
  --print(entity:getName());
  --print(entity:parent():getTeam());
  --entity:setCurrentScore(self:getScore());
end




