<?php

	
	 
?>
<?php
include_once("stat_function.php");
include_once("stat_game.php");
include_once("stat_map_graph_display.php");

	$cacheFileName = $cache_dir."/stat_global.html";
	
	if(isCacheFileUpToDate($cacheFileName))
	{
		include($cacheFileName);
		return;
	}
	
	$html_fp = fopen($cacheFileName, "wt");

	/*
	TODO : user list(sorted)
	TODO : game by day graph
	*/
	fprintf($html_fp,"<script type='text/javascript' src='js/switchcontent.js'></script>");
	fprintf($html_fp,"<b><a href=\"javascript:toggleElementByName('expandable')\">expand stats</a></b>");

	$sessionCount = 10;
	$requete = "SELECT Id FROM session ORDER BY Id DESC LIMIT 0,$sessionCount";
	drawGames($html_fp,$requete,"Last $sessionCount games",-1);

	//$requete = "SELECT count(*), HOUR(Date) as h FROM session GROUP BY HOUR(Date);";
	$requete = "SELECT count(*), HOUR(Date) as c FROM session WHERE TO_DAYS(NOW()) = TO_DAYS(Date) GROUP BY c;";
	$result=exec_game_db_requete($requete);
	drawGraph($html_fp,$result,true,0,24,"Today games by hour","","Hour");

	$requete = "SELECT count(*), HOUR(Date) as c FROM session WHERE TO_DAYS(NOW())-TO_DAYS(session.Date)<=29 GROUP BY c;";
	$result=exec_game_db_requete($requete);
	drawGraph($html_fp,$result,true,0,24,"Last 30 days games by hour","","Hour");

	$yearToStat = thisYear();
	$link = "<a href=\"?page=stat_day.php&p_day_to_stat=%s&p_month_to_stat=%s&p_year_to_stat=%s\">%s";
	$requete = "SELECT count(*), 29-(TO_DAYS(NOW())-TO_DAYS(session.Date)),DAYOFMONTH(session.Date), MONTH(session.Date) , YEAR(session.Date) FROM session WHERE TO_DAYS(NOW())-TO_DAYS(session.Date)<=29 GROUP BY TO_DAYS(session.Date);";
	$result=exec_game_db_requete($requete);
	drawGraphMultipleLinkCount($html_fp,$result,false,0,30,"Last 30 days games by day","",array("", "Day","Month"),true,$link,3);

	$link = "<a href=\"?page=stat_month.php&p_month_to_stat=%s&p_year_to_stat=%s\">%s";
	$requete = "SELECT count(*), MONTH(Date), MONTH(Date) as c , YEAR(Date) FROM session WHERE TO_DAYS(NOW())-TO_DAYS(session.Date)<=365 GROUP BY c;";
	$result=exec_game_db_requete($requete);
	drawGraphMultipleLinkCount($html_fp,$result,false,1,12,"Last 12 months games","",array("","Month"),true,$link,2);

	$requete = "SELECT count(*),map.Id,map.LevelName FROM session,map WHERE map.LevelName=session.LevelName GROUP BY session.LevelName;";
	$result=exec_game_db_requete($requete);
	drawMapUsage($html_fp,$result,1,15,"Level usage",array("", "Level"),"<a href=\"?page=stat_map.php&p_map_id=%d\">%s</a>");
	
	$requete = "SELECT count(*),shard.ShardId,shard.Name FROM session,shard WHERE shard.ShardId=session.SId GROUP BY session.SId;";
	$result=exec_game_db_requete($requete);
	drawMapUsage($html_fp,$result,1,15,"server usage",array("", "Server"),"<a href=\"?page=stat_server.php&p_server_id=%d\">%s</a>");
	
	


	fclose($html_fp);	  
	include($cacheFileName);	
?>
	