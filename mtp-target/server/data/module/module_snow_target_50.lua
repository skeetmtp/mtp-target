snow_target_50_parent = {}

function snow_target_50_parent:show() 
  print("snow_target_50_parent show");
end



print(module:getName());
print("module load");

SetInheritance(module,snow_target_50_parent);


module:setBounce(1);
module:setScore(50);
module:setAccel(0.0001);
module:setFriction(5.0);
module:setShapeName("snow_target_50.shape");
