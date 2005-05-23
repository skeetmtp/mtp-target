<?php
require_once("stat_function.php");
require_once("stat_game.php");
require_once("stat_map_graph_display.php");

	$p_user_id = $_GET['p_user_id'];

	assertValidNumber($p_user_id);

	$uid = $p_user_id;
	$user_name = userId2Name($uid);
	$user_team = getUserTeam($user_name);
	$cacheFileName = $cache_dir."/stat_user_".$uid.".html";
	printf("<center><b>%s</b> stats : </center><br/<br/>>",$user_name);

	if(isCacheFileUpToDate($cacheFileName))
	{
		require_once($cacheFileName);
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
		fprintf($html_fp,"%s have never played<br/>\n",$user_name);
		fclose($html_fp);	  
		require_once($cacheFileName);
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
	$userTextureUploader = textureGetUploader($userTextureName);
	$userTextureUrl = $userTexturePath.$userTextureStartName.$userTextureName.".jpg";
	fprintf($html_fp,"<img src=\"%s\" ALT=\"user texture : %s'\"><br/>\n",$userTextureUrl,$userTextureUrl);
	fprintf($html_fp,"</td>\n");

	fprintf($html_fp,"<td valign=\"bottom\">\n");
	fprintf($html_fp,"Registred for %d days<br/>\n",$userRegistrationDateInDays);
	fprintf($html_fp,"First game : %s<br/>\n",$userFirstSessionDate);
	fprintf($html_fp,"Last game : %s<br/>\n",$userLastSessionDate);
	fprintf($html_fp,"Rank : %s<br/>\n",$userRank);
	fprintf($html_fp,"total score = %s<br/>\n",hugeNumber2Human($totalScore));
	//fprintf($html_fp,"sum score = %s<br/>\n",hugeNumber2Human($currentScore));
	fprintf($html_fp,"Team : <b><a href=\"?page=stat_team&p_team=%s\">%s</a></b><br/>\n",$user_team,$user_team);
	
	fprintf($html_fp,"%d sessions played<br>\n",$sessionPlayedCount);
	fprintf($html_fp,"have played during %s<br/>\n",timeInSeconds2HumanDuration($sessionPlayedDuration));
	fprintf($html_fp,"about %.0f seconds per game<br/>\n",$sessionPlayedDuration/$sessionPlayedCount);
	fprintf($html_fp,"about %.0f points per game<br/>\n",$totalScore/$sessionPlayedCount);
	fprintf($html_fp,"about %.0f%% successful landing<br/>\n",100.0*$sessionWithScoreCount/$sessionPlayedCount);
	if($userTextureUploader=="")
		fprintf($html_fp,"user texture : %s",$userTextureName);
	else
		fprintf($html_fp,"user texture : %s (uploaded by %s)",$userTextureName,$userTextureUploader);
	fprintf($html_fp,"</td>\n");

	fprintf($html_fp,"</tr>\n");
	fprintf($html_fp,"</table>\n");
	
	
	fprintf($html_fp,"<br/>\n");
	fprintf($html_fp,"<br/>\n");

	
	
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

	$yearToStat = thisYear();
	$link = "<a href=\"?page=stat_day&p_uid=$uid&p_day_to_stat=%s&p_month_to_stat=%s&p_year_to_stat=%s\">%s";
	$requete = "SELECT count(*), 29-(TO_DAYS(NOW())-TO_DAYS(session.Date)),DAYOFMONTH(session.Date), MONTH(session.Date), YEAR(session.Date) FROM session,user_session WHERE TO_DAYS(NOW())-TO_DAYS(session.Date)<=29 AND session.Id=user_session.SessionId AND user_session.UId=$uid GROUP BY TO_DAYS(session.Date);";
	$result=exec_game_db_requete($requete);
	//drawGraph($html_fp,$result,false,0,30,"Last 30 days games by day","","Day");
	drawGraphMultipleLinkCount($html_fp,$result,false,0,30,"Last 30 days games by day","",array("", "Day","Month"),true,$link,3);
	
	$link = "<a href=\"?page=stat_month&p_uid=$uid&p_month_to_stat=%s&p_year_to_stat=%s\">%s";
	$requete = "SELECT count(*), MONTH(session.Date), MONTH(session.Date) as c, YEAR(session.Date) FROM session,user_session WHERE session.Id=user_session.SessionId AND TO_DAYS(NOW())-TO_DAYS(session.Date)<=365 AND user_session.UId=$uid GROUP BY c;";
	$result=exec_game_db_requete($requete);
	drawGraphMultipleLinkCount($html_fp,$result,false,1,12,"Last 12 months games","",array("", "Month"),true,$link,2);

	$requete = "SELECT count(*),map.Id,map.LevelName FROM session,map,user_session WHERE map.LevelName=session.LevelName AND session.Id=user_session.SessionId AND user_session.UId=$uid GROUP BY session.LevelName;";
	$result=exec_game_db_requete($requete);
	drawMapUsage($html_fp,$result,1,32,"Level usage",array("", "Level"),"<a href=\"?page=stat_map&p_map_id=%d\">%s</a>");
	

	//fprintf($html_fp,"TODO:Maximum loss in a row<br/>\n");
	//fprintf($html_fp,"TODO:Maximum win in a row<br/>\n");

	drawTopPlayedUser($html_fp,$uid);
	
	/*
	fprintf($html_fp,"Stats by level :<br/>");
	$requete = "SELECT * FROM map;";
	$resultMapList=exec_game_db_requete($requete);
	while($mapList = mysql_fetch_array($resultMapList))
	{
		$mapName = $mapList[1]; 
		fprintf($html_fp,"<a href=\"?page=stat_map_user&p_map_id=%d&p_user_id=%d\">%s</a><br/>\n",$mapList[0],$uid,$mapName);
	}
	
	fprintf($html_fp,"<br/>");
	fprintf($html_fp,"<br/>");
	*/
	
	fclose($html_fp);	  
	
	require_once($cacheFileName);	
?>