<?php
include_once("stat_function.php");
include_once("stat_game.php");
include_once("helpers.php");

	$cacheFileName = $cache_dir."/stat_player_of_the_day.html";
	if(isCacheFileUpToDate($cacheFileName))
	{
		include($cacheFileName);
		return;
	}

	$html_fp = fopen($cacheFileName, "wt");


	boxBeginFp($html_fp,true);
	$requete = "SELECT user.Login,SUM(user_session.Score) as s,user.UId FROM session,user_session,user WHERE session.Id=user_session.SessionId AND user_session.UId=user.UId AND TO_DAYS(NOW())-TO_DAYS(session.Date)=0 GROUP BY user_session.UId ORDER BY s DESC LIMIT 0,1 ;";
	$result=exec_game_db_requete($requete);
	if($line = mysql_fetch_array($result))
	{
		fprintf($html_fp,"<center>");
		fprintf($html_fp,"Player of the day :<br>\n");
		fprintf($html_fp,"<b><a href=\"?page=stat_user.php&p_user_id=%d\">%s</a></b> with %s points today<br>\n",$line[2],$line[0],$line[1]);
		fprintf($html_fp,"</center>");
		fprintf($html_fp,"<br>");
	}


	$requete = "SELECT * FROM user ORDER BY UId DESC LIMIT 0,1 ;";
	$result=exec_game_db_requete($requete);
	if($line = mysql_fetch_array($result))
	{
		fprintf($html_fp,"<div id=\"small\">");
		fprintf($html_fp,"The newest registered user is <b><a href=\"?page=stat_user.php&p_user_id=%d\">%s</a></b>\n",$line[0],$line[1]);
		fprintf($html_fp,"</div>");
	}
	boxEndFp($html_fp);


	fclose($html_fp);	  
	include($cacheFileName);	
?>
