---------------------- Global  ----------------------



---------------------- Entity ----------------------
local clientId = 0;

function Entity:init()
  self:setCurrentScore(0);
  self:setStartPointId(clientId+getSessionId()*2);
  clientId = clientId + 1;
  --self:setMaxLinearVelocity(0.2);
end

function Entity:preUpdate()
  self:setCurrentScore(0);
end

function Entity:update()
end

function entitySceneCollideEvent ( entity, module )
  module:parent():onCollide(entity);
end

function entityEntityCollideEvent ( entity1, entity2 )
end

function entityWaterCollideEvent ( entity )
  pos = entity:getStartPointPos();
  --entity:setPos(pos);
  --entity:setOpenCloseCount(0);
end

---------------------- Module ----------------------

function Module:parent()
  return self:getUserData();
end


---------------------- Level  ----------------------

function levelPreUpdate()
  local moduleCount = getModuleCount();
  for i=0,moduleCount-1 do
    if(getModule(i):parent()) then
      getModule(i):parent():preUdate();
    end
  end
end

function levelPostUpdate()
  local moduleCount = getModuleCount();
  for i=0,moduleCount-1 do
    if(getModule(i):parent()) then
      getModule(i):parent():postUdate();
    end
  end
end

function levelEndSession()
  local moduleCount = getModuleCount();
  for i=0,moduleCount-1 do
    if(getModule(i):parent()) then
      getModule(i):parent():finalScore();
    end
  end
end


