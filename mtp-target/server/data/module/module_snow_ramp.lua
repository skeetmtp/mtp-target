snow_ramp_parent = {}

function snow_ramp_parent:show() 
  print("snow_ramp_parent show");
end

function snow_ramp_parent:init() 
--  print(self:getName());
--  print("module init");
end

function snow_ramp_parent:update()
--  print(self:getName());
--  print("module update");
end

print(module:getName());
print("module load");


SetInheritance(module,snow_ramp_parent);

module:setAccel(0.0001);
module:setFriction(0);
module:setBounce(0);
module:setShapeName("snow_ramp.shape");



