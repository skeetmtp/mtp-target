<?php
include_once("stat_function.php");
include_once("stat_game.php");
include_once("stat_map_graph_display.php");

	assertValidNumber($p_user_id);

	$uid = $p_user_id;
	$user_name = userId2Name($uid);
	$cacheFileName = $cache_dir."/stat_user_".$uid.".html";
	printf("<center><b>%s</b> stats : </center><br><br>",$user_name);
	
	
	if(isCacheFileUpToDate($cacheFileName))
	{
		include($cacheFileName);
		return;
	}
	
	
	
	$html_fp = fopen($cacheFileName, "wt");

	$totalScore = 1;
	$currentScore = 1;
	$sessionPlayedCount = 1;
	$sessionPlayedDuration = 1;
	$sessionWithScoreCount = 1;
	
	userOverallStat($uid,$totalScore,$currentScore,$sessionPlayedCount,$sessionPlayedDuration);
	if($sessionPlayedCount<=0)
	{
		fprintf($html_fp,"%s have never played<br>\n",$user_name);
		fclose($html_fp);	  
		include($cacheFileName);
		return;
	}
	
	$sessionWithScoreCount = userTotalSessionWithPointCount($uid);
	$userRank = userRank($totalScore);
	$userRegistrationDateInDays = userRegistrationDateInDays($uid);
	$userFirstSessionDate = userFirstSessionDate($uid);
	$userLastSessionDate = userLastSessionDate($uid);

	
	fprintf($html_fp,"<table border=\"0\" cellpadding=\"5\" cellspacing=\"1\" >\n");
	fprintf($html_fp,"<tr>\n");

	fprintf($html_fp,"<td >\n");
	$userTextureName = userTexture($uid);
	$userTextureUrl = $userTexturePath.$userTextureStartName.$userTextureName.".jpg";
	fprintf($html_fp,"<img src=\"%s\" ALT=\"user texture : %s\"><br>\n",$userTextureUrl,$userTextureUrl);
	fprintf($html_fp,"</td>\n");

	fprintf($html_fp,"<td valign=\"bottom\">\n");
	fprintf($html_fp,"Registred for %d days<br>\n",$userRegistrationDateInDays);
	fprintf($html_fp,"First game : %s<br>\n",$userFirstSessionDate);
	fprintf($html_fp,"Last game : %s<br>\n",$userLastSessionDate);
	fprintf($html_fp,"Rank : %s<br>\n",$userRank);
	fprintf($html_fp,"total score = %s<br>\n",hugeNumber2Human($totalScore));
	
	fprintf($html_fp,"%d sessions played<br>\n",$sessionPlayedCount);
	fprintf($html_fp,"have played during %s<br>\n",timeInSeconds2HumanDuration($sessionPlayedDuration));
	fprintf($html_fp,"about %.0f seconds per game<br>\n",$sessionPlayedDuration/$sessionPlayedCount);
	fprintf($html_fp,"about %.0f points per game<br>\n",$totalScore/$sessionPlayedCount);
	fprintf($html_fp,"about %.0f%% successful landing<br>\n",100.0*$sessionWithScoreCount/$sessionPlayedCount);
	fprintf($html_fp,"</td>\n");

	fprintf($html_fp,"</tr>\n");
	fprintf($html_fp,"</table>\n");
	
	
	fprintf($html_fp,"<br>\n");
	fprintf($html_fp,"<br>\n");

	
	
	fprintf($html_fp,"<script type='text/javascript' src='js/switchcontent.js'></script>");
	fprintf($html_fp,"<b><a href=\"javascript:toggleElementByName('expandable')\">expand stats</a></b>");

	$sessionCount = 5;
	$requete = "SELECT session.Id FROM session,user_session WHERE user_session.UId=$uid AND session.Id=user_session.SessionId ORDER BY user_session.Id DESC LIMIT 0,$sessionCount";
	drawGames($html_fp,$requete,"$user_name last $sessionCount games",$uid);
	
	$requete = "SELECT count(*), HOUR(session.Date) as c FROM session,user_session WHERE TO_DAYS(NOW())=TO_DAYS(session.Date) AND session.Id=user_session.SessionId AND user_session.UId=$uid GROUP BY c;";
	$result=exec_game_db_requete($requete);
	drawGraph($html_fp,$result,true,0,24,"Today games by hour","","Hour");

	$requete = "SELECT count(*), HOUR(session.Date) as c FROM session,user_session WHERE TO_DAYS(NOW())-TO_DAYS(session.Date)<=30 AND YEAR(NOW())=YEAR(Date) AND session.Id=user_session.SessionId AND user_session.UId=$uid GROUP BY c;";
	$result=exec_game_db_requete($requete);
	drawGraph($html_fp,$result,true,0,24,"Last 30 days games by hour","","Hour");

	$yearToStat = 2004;
	$link = "<a href=\"?page=stat_day.php&p_uid=$uid&p_year_to_stat=$yearToStat&p_day_to_stat=%s&p_month_to_stat=%s\">%s";
	$requete = "SELECT count(*), 29-(TO_DAYS(NOW())-TO_DAYS(session.Date)),DAYOFMONTH(session.Date), MONTH(session.Date) FROM session,user_session WHERE TO_DAYS(NOW())-TO_DAYS(session.Date)<=29 AND session.Id=user_session.SessionId AND user_session.UId=$uid GROUP BY TO_DAYS(session.Date);";
	$result=exec_game_db_requete($requete);
	//drawGraph($html_fp,$result,false,0,30,"Last 30 days games by day","","Day");
	drawGraphMultipleLink($html_fp,$result,false,0,30,"Last 30 days games by day","",array("", "Day","Month"),true,$link);
	
	$link = "<a href=\"?page=stat_month.php&p_uid=$uid&p_year_to_stat=$yearToStat&p_month_to_stat=%s\">%s";
	$requete = "SELECT count(*), MONTH(session.Date) as c FROM session,user_session WHERE session.Id=user_session.SessionId AND TO_DAYS(NOW())-TO_DAYS(session.Date)<=365 AND user_session.UId=$uid GROUP BY c;";
	$result=exec_game_db_requete($requete);
	drawGraphLink($html_fp,$result,false,1,12,"Last 12 months games","","Month",$link);

	$requete = "SELECT count(*),map.Id,map.LevelName FROM session,map,user_session WHERE map.LevelName=session.LevelName AND session.Id=user_session.SessionId AND user_session.UId=$uid GROUP BY session.LevelName;";
	$result=exec_game_db_requete($requete);
	drawMapUsage($html_fp,$result,1,15,"Level usage",array("", "Level"));
	

	//fprintf($html_fp,"TODO:Maximum loss in a row<br>\n");
	//fprintf($html_fp,"TODO:Maximum win in a row<br>\n");

	drawTopPlayedUser($html_fp,$uid);
	
	/*
	fprintf($html_fp,"Stats by level :<br>");
	$requete = "SELECT * FROM map;";
	$resultMapList=exec_game_db_requete($requete);
	while($mapList = mysql_fetch_array($resultMapList))
	{
		$mapName = $mapList[1]; 
		fprintf($html_fp,"<a href=\"?page=stat_map_user.php&p_map_id=%d&p_user_id=%d\">%s</a><br>\n",$mapList[0],$uid,$mapName);
	}
	
	fprintf($html_fp,"<br>");
	fprintf($html_fp,"<br>");
	*/
	
	fclose($html_fp);	  
	
	include($cacheFileName);	
?>