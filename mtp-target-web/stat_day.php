<?php
include_once("stat_function.php");
include_once("stat_game.php");

	if(!isset($p_uid) || $p_uid==0)
		$uid = 0;
	else
	{
		assertValidNumber($p_uid);
		$uid = $p_uid;
		$user_name = userId2Name($uid);
	}
	assertValidNumber($p_year_to_stat);
	assertValidNumber($p_month_to_stat);
	assertValidNumber($p_day_to_stat);
	$yearToStat = $p_year_to_stat;
	$monthToStat = $p_month_to_stat;
	$dayToStat = $p_day_to_stat;

	$cacheFileName = $cache_dir."/stat_day_".$uid."_".$yearToStat."_".$monthToStat."_".$dayToStat.".html";

	if(isCacheFileUpToDate($cacheFileName))
	{
		include($cacheFileName);
		return;
	}
	
	$html_fp = fopen($cacheFileName, "wt");

	fprintf($html_fp,"<script type='text/javascript' src='js/switchcontent.js'></script>");
	fprintf($html_fp,"<b><a href=\"javascript:toggleElementByName('expandable')\">expand stats</a></b>");
	
	$dayString = date("Y-m-d", mktime(0, 0, 0, $monthToStat, $dayToStat, $yearToStat)); 
	if($uid==0)	
		$requete = "SELECT count(*), HOUR(Date) as c FROM session WHERE TO_DAYS('$dayString')=TO_DAYS(session.Date) GROUP BY c;";
	else
		$requete = "SELECT count(*), HOUR(Date) as c FROM session,user_session WHERE TO_DAYS('$dayString')=TO_DAYS(session.Date) AND session.Id=user_session.SessionId AND user_session.UId=$uid GROUP BY c;";
	$result=exec_game_db_requete($requete);
	if($uid==0)
	{	
		$link = "<a href=\"?page=stat_hour.php&p_uid=$uid&p_year_to_stat=$yearToStat&p_month_to_stat=$monthToStat&p_day_to_stat=$dayToStat&p_hour_to_stat=%s\">%s";
		drawGraphLink($html_fp,$result,true,0,24,sprintf("%s : games by hour",date("M-d-Y", mktime(0, 0, 0, $monthToStat, $dayToStat, $yearToStat))),"","Hour",$link);
	}
	else
	{
		$link = "<a href=\"?page=stat_hour.php&p_year_to_stat=$yearToStat&p_month_to_stat=$monthToStat&p_day_to_stat=$dayToStat&p_hour_to_stat=%s\">%s";
		drawGraphLink($html_fp,$result,true,0,24,sprintf("%s : $user_name games by hour",date("M-d-Y", mktime(0, 0, 0, $monthToStat, $dayToStat, $yearToStat))),"","Hour",$link);
	}

	if($user_id==0)
	{	
		$requete = "SELECT MAX(NbPlayers), HOUR(Date) as c FROM session WHERE TO_DAYS('$dayString')=TO_DAYS(session.Date) GROUP BY c;";
		$result=exec_game_db_requete($requete);
		drawGraphLink($html_fp,$result,true,0,24,sprintf("%s : max players on ONE server by hour",date("M-d-Y", mktime(0, 0, 0, $monthToStat, $dayToStat, $yearToStat))),"","Hour",$link);
	}


	fclose($html_fp);	  
	include($cacheFileName);	
?>
