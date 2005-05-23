<?php
require_once("stat_function.php");
require_once("stat_game.php");

	if(!isset($p_uid))
		$uid = 0;
	else
	{
		assertValidNumber($p_uid);
		$uid = $p_uid;
	}
	assertValidNumber($p_year_to_stat);
	assertValidNumber($p_month_to_stat);
	assertValidNumber($p_day_to_stat);
	assertValidNumber($p_hour_to_stat);
	$yearToStat = $p_year_to_stat;
	$monthToStat = $p_month_to_stat;
	$dayToStat = $p_day_to_stat;
	$hourToStat = $p_hour_to_stat;

	$cacheFileName = $cache_dir."/stat_hour_".$uid."_".$yearToStat."_".$monthToStat."_".$dayToStat."_".$hourToStat.".html";

	if(isCacheFileUpToDate($cacheFileName))
	{
		require_once($cacheFileName);
		return;
	}
	
	$html_fp = fopen($cacheFileName, "wt");

	fprintf($html_fp,"<script type='text/javascript' src='js/switchcontent.js'></script>");
	fprintf($html_fp,"<b><a href=\"javascript:toggleElementByName('expandable')\">expand stats</a></b>");
	
	/*
	$yearToStat = 2004;
	$monthToStat = 7;
	$dayToStat = 9;
	$hourToStat = 20;
	*/

	$requete = sprintf("SELECT MAX(NbPlayers), SId FROM session WHERE TO_DAYS('%s')=TO_DAYS(session.Date) AND HOUR(Date)=%d GROUP BY SId;",date("Y-m-d", mktime(0, 0, 0, $monthToStat, $dayToStat, $yearToStat)),$hourToStat);
	$result=exec_game_db_requete($requete);
	drawGraph($html_fp,$result,false,1,10,sprintf("%s , %dh : max players logged at once by server",date("M-d-Y", mktime(0, 0, 0, $monthToStat, $dayToStat, $yearToStat)),$hourToStat),"","Server Id");

	$requete = sprintf("SELECT MAX(NbPlayers), SId FROM session WHERE TO_DAYS('%s')=TO_DAYS(session.Date) AND HOUR(Date)=%d GROUP BY SId;",date("Y-m-d", mktime(0, 0, 0, $monthToStat, $dayToStat, $yearToStat)),$hourToStat);
	$result=exec_game_db_requete($requete);
	$count = 0;
	while($line = mysql_fetch_array($result))
	{
		$count += $line[0];
	}
	fprintf($html_fp,"max players logged this hour : %s<br/>\n",$count);

	$sessionCount = 10;
	$requete = sprintf("SELECT Id FROM session WHERE TO_DAYS('%s')=TO_DAYS(session.Date) AND HOUR(Date)=%d ORDER BY Id DESC;",date("Y-m-d", mktime(0, 0, 0, $monthToStat, $dayToStat, $yearToStat)),$hourToStat);
	//$requete = "SELECT Id FROM session ORDER BY Id DESC";
	drawGames($html_fp,$requete,sprintf("%s , %dh : games",date("M-d-Y", mktime(0, 0, 0, $monthToStat, $dayToStat, $yearToStat)),$hourToStat),-1);



	fclose($html_fp);	  
	require_once($cacheFileName);	
?>
