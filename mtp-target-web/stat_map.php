<?php
require_once("stat_function.php");

	//$fullUrl = "http://{$HTTP_HOST}{$REQUEST_URI}";
	if(!isset($p_map_id))
		exit();
		//$p_map_id = 1;
	assertValidNumber($p_map_id);
	$map_id = $p_map_id;
	$mapName = mapId2Name($map_id);
	$cacheFileName = $cache_dir."/stat_map_".$map_id.".html";
	printf("<center><b>%s</b> stats : </center><br/<br/>>",$mapName);

	
	if(isCacheFileUpToDate($cacheFileName))
	{
		require_once($cacheFileName);
		return;
	}
	
	$html_fp = fopen($cacheFileName, "wt");

	$descMapFilename = "./lang/".CUser::instance()->language()."/".$mapName.".php";
	if(file_exists($descMapFilename))
	require_once($descMapFilename);

	printf("<img src=\"$sshot_dir/%s.jpg\" ALT=\"%s screenshot \"><br/>\n",$mapName,$mapName);


	fclose($html_fp);	  
	require_once($cacheFileName);	
?>