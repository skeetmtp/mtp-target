<?php
include_once("stat_function.php");
include_once("stat_game.php");
include_once("stat_graph_display.php");

	//$uid = userName2Uid($user_login);
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

	$requete = sprintf("SELECT MAX(NbPlayers), HOUR(session.Date) as h FROM session WHERE TO_DAYS(NOW())=TO_DAYS(session.Date) AND session.SId=$server_id GROUP BY h;");
	$result=exec_game_db_requete($requete);
	drawGraph($html_fp,$result,true,1,24,sprintf("Today : max players logged at once"),"","Hour");

	$yearToStat = thisYear();
	$requete = "SELECT MAX(NbPlayers), 29-(TO_DAYS(NOW())-TO_DAYS(session.Date)),DAYOFMONTH(session.Date), MONTH(session.Date) FROM session WHERE TO_DAYS(NOW())-TO_DAYS(session.Date)<=29  AND session.SId=$server_id GROUP BY TO_DAYS(session.Date);";
	$result=exec_game_db_requete($requete);
	drawGraphMultiple($html_fp,$result,false,0,30,"Last 30 days : max players logged at once","",array("", "Day","Month"),true);


	fclose($html_fp);	  
	include($cacheFileName);	
?>