<?php
include_once("stat_function.php");
include_once("stat_game.php");
include_once("stat_graph_display.php");

	$uid = userName2Uid($user_login);
	assertValidNumber($p_server_id);
	$server_id = $p_server_id;
	$serverName = Id2ServerName($server_id);
	$cacheFileName = $cache_dir."/stat_server_".$server_id.".html";
	printf("<center><b>%s</b> stats : </center><br><br>",$serverName);
	
	
	if(isCacheFileUpToDate($cacheFileName))
	{
		include($cacheFileName);
		return;
	}
	
	
	$html_fp = fopen($cacheFileName, "wt");
	
	fprintf($html_fp,"<script type='text/javascript' src='js/switchcontent.js'></script>");
	fprintf($html_fp,"<b><a href=\"javascript:toggleElementByName('expandable')\">expand stats</a></b>");



	fclose($html_fp);	  
	include($cacheFileName);	
?>