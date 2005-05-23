<?php

	$cacheFileName = $cache_dir."/stat_map_list.html";
	
	if(isCacheFileUpToDate($cacheFileName))
	{
		require_once($cacheFileName);
		return;
	}
	
	$html_fp = fopen($cacheFileName, "wt");
	$requete = "SELECT * FROM map;";
	$resultat=exec_game_db_requete($requete);
	
	while($ligne = mysql_fetch_array($resultat))
	{
		fprintf($html_fp,"<b><a href=\"?page=stat_map&p_map_id=%d\">%s</a></b><br/>\n",$ligne[0],$ligne[1]);
	}
	fclose($html_fp);	  
	
	require_once($cacheFileName);	
?>