<?php

	$cacheFileName = $cache_dir."/stat_map_list.html";
	
	if(isCacheFileUpToDate($cacheFileName))
	{
		include($cacheFileName);
		return;
	}
	
	$html_fp = fopen($cacheFileName, "wt");
	$requete = "SELECT DISTINCT LevelName FROM session;";
	$resultat=exec_game_db_requete($requete);
	
	while($ligne = mysql_fetch_array($resultat))
	{
		fprintf($html_fp,"%s<br>\n",$ligne[0]);
	}
	fclose($html_fp);	  
	
	include($cacheFileName);	
?>