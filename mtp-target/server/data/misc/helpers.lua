SetInheritance0 = function(obj, parent)
  obj:setUserData(parent);
end

SetInheritance = function(obj, parent)
  obj:setUserData(parent);

  local pmt = getmetatable(parent)
  
  local nmt = {}
  local omt = getmetatable(obj)
  local oldIndex = omt.__index;

  nmt = omt;

  nmt.__index = function(object, key)
    local result1
    local result
    local parent;

    if type(oldIndex) == "table" then
      result1 = oldIndex[key]
    else
      result1 = oldIndex(object, key)
    end
    
    if result1 then 
    	return result1
    end
      
    if type(oldIndex) == "table" then
      result = oldIndex["getUserData"]
    else
      result = oldIndex(object, "getUserData")
    end
    
    if(result) then 
      parent = result(object)
    end
    
    if(parent) then
      result = parent[key];
    end

    return result
  end

  --obj:setMetatable(obj,nmt)

end

SetInheritanceOLD = function(obj, parent)
  local nmt = {}
  local omt = getmetatable(obj)
  
  table.foreach(omt,warn);
  
  local oldIndex = omt.__index;
  nmt = omt;

  nmt.__index = function(object, key)
    local result
   
    if (omt) then
      -- There should be a simpler way to
      -- express this lookup.
      --result = oldIndex[key]
      
      if type(oldIndex) == "table" then
        result = oldIndex[key]
      else
        result = oldIndex(object, key)
      end
    end

    if result then 
    	return result 
    end

    return parent[key]
  end

  -- Presumably we need a C function to call
  -- setmetatable for us because Lua code
  -- isn't allowed to set metatables for
  -- a userdata
  
  
  --print("omt = ");
  --table.foreach(omt,warn);
  --print("nmt = ");
  --table.foreach(nmt,warn);
  

  obj:setMetatable(obj,nmt)
end
