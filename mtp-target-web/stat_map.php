<?php
include_once("stat_function.php");

	assertValidNumber($p_map_id);
	$map_id = $p_map_id;
	$mapName = mapId2Name($map_id);
	$cacheFileName = $cache_dir."/stat_map_".$map_id.".html";
	printf("<center><b>%s</b> stats : </center><br><br>",$mapName);

	
	if(isCacheFileUpToDate($cacheFileName))
	{
		include($cacheFileName);
		return;
	}
	
	$html_fp = fopen($cacheFileName, "wt");


	printf("<img src=\"$sshot_dir/%s.jpg\" ALT=\"%s screenshot \"><br>\n",$mapName,$mapName);


	fclose($html_fp);	  
	include($cacheFileName);	
?>