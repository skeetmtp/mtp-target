<?php
include_once("stat_function.php");
include_once("stat_game.php");

	$user_name = "";
	if(!isset($p_uid))
		$uid = 0;
	else
	{
		assertValidNumber($p_uid);
		$uid = $p_uid;
		$user_name = userId2Name($uid);
	}
	assertValidNumber($p_year_to_stat);
	assertValidNumber($p_month_to_stat);
	$yearToStat = $p_year_to_stat;
	$monthToStat = $p_month_to_stat;

	$cacheFileName = $cache_dir."/stat_month_".$uid."_".$yearToStat."_".$monthToStat.".html";
	
	if(isCacheFileUpToDate($cacheFileName))
	{
		include($cacheFileName);
		return;
	}
	
	$html_fp = fopen($cacheFileName, "wt");
	fprintf($html_fp,"<script type='text/javascript' src='js/switchcontent.js'></script>");
	fprintf($html_fp,"<b><a href=\"javascript:toggleElementByName('expandable')\">expand stats</a></b>");

	//$link = "<a href=\"?page=stat_day.php&p_uid=$uid&p_year_to_stat=$yearToStat&p_month_to_stat=$monthToStat&p_day_to_stat=%s\">%s";
	$link = "<a href=\"?page=stat_day.php&p_uid=$uid&p_year_to_stat=$yearToStat&p_month_to_stat=$monthToStat&p_day_to_stat=%s\">%s";
	if($uid==0)	
		$requete = "SELECT count(*),DAYOFMONTH(session.Date) as d1 ,DAYOFMONTH(session.Date) as d2 FROM session WHERE MONTH(session.Date)=$monthToStat GROUP BY TO_DAYS(session.Date);";
	else
		$requete = "SELECT count(*),DAYOFMONTH(session.Date) as d1 ,DAYOFMONTH(session.Date) as d2 FROM session,user_session WHERE MONTH(session.Date)=$monthToStat AND session.Id=user_session.SessionId AND user_session.UId=$uid GROUP BY TO_DAYS(session.Date);";
	$result=exec_game_db_requete($requete);
	drawGraphMultipleLink($html_fp,$result,false,1,31,sprintf("%s : $user_name games by day",date("F Y",mktime(0,0,0,$monthToStat))),"",array("", "Day"),true,$link);

/*
	$requete = sprintf("SELECT count(*), HOUR(Date) as c FROM session WHERE TO_DAYS('%s')-TO_DAYS(session.Date)<=29 GROUP BY c;",date("Y-m-d", mktime(0, 0, 0, $monthToStat, $dayToStat, $yearToStat)));
	$result=exec_game_db_requete($requete);
	drawGraph($html_fp,$result,true,0,24,sprintf("%s : games by hour",date("M-d-Y", mktime(0, 0, 0, $monthToStat, $dayToStat, $yearToStat))),"","Hour");
*/

	fclose($html_fp);	  
	include($cacheFileName);	
?>
