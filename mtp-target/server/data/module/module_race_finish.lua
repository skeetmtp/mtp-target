module:setBounce(0);
module:setCollide(0);
module:setScore(0);
module:setAccel(0.00001);
module:setFriction(1.0);

CModuleFinish = {}
CModuleFinish_mt = {}
function CModuleFinish:new(s)
  return setmetatable({ son=s }, CModuleFinish_mt)
end


CModuleFinish_mt.__index = CModuleFinish

function CModuleFinish:onCollide( entity )
  if(entity:parent().team == 1) then
    entity:displayText(0,5,1,255,255,0,"Finish",3);
    entity:setCurrentScore(getTimeRemaining()*10);
    entity:displayText(0,6,1,255,255,0,entity:getCurrentScore(),3);
    entity:parent().team = 2;
    entity:setOpenCloseCount(2);
    --entity:setFreezCommand(1);
  end
end

function CModuleFinish:onInit()
  self.son:setVisible(0);
end


module:setUserData(CModuleFinish:new(module)); --new CModule
