<?php
include_once("stat_function.php");
include_once("stat_game.php");

	//$uid = userName2Uid($user_login);
	//$uid = 5453;
	$cacheFileName = $cache_dir."/test.html";
	
	
	printf("<center><b>Online server list</b></center><br>\n");
	
	if(isCacheFileUpToDateDuration($cacheFileName,30))
	{
		include($cacheFileName);
		return;
	}
	
	$html_fp = fopen($cacheFileName, "wt");

	$requete = "SELECT * FROM shard WHERE State='Online';";
	$result=exec_game_db_requete($requete);
	while($line = mysql_fetch_array($result))
	{
		fprintf($html_fp,"id:%d <b><a href=\"?page=stat_server.php&p_server_id=%d\">%s</a></b> %d users<br>\n",$line[0],$line[0],$line[3],$line[2]);
	}

	fclose($html_fp);	  
	include($cacheFileName);	
?>
