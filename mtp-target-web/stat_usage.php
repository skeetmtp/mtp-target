<?php
include_once("stat_function.php");
include_once("stat_game.php");

	$uid = userName2Uid($user_login);
	//$uid = 5453;
	$cacheFileName = $cache_dir."/".$page.".html";
	
	/*
	if(isCacheFileUpToDate($cacheFileName))
	{
		include($cacheFileName);
		return;
	}
	*/
	
	$html_fp = fopen($cacheFileName, "wt");


	$monthToCompute = 7;
	$requete = "SELECT COUNT(DISTINCT user_session.UId) FROM session,user_session WHERE session.Id=user_session.SessionId AND MONTH(session.Date)=$monthToCompute AND YEAR(NOW())=YEAR(session.Date);";
	$result=exec_game_db_requete($requete);
	if($line = mysql_fetch_array($result))
		$totalUCount = $line[0];
	
	$ucount = 0;
	$requete = "SELECT SUM(session.Duration) as s,user.Login FROM session,user_session,user WHERE user.UId=user_session.UId AND session.Id=user_session.SessionId AND MONTH(session.Date)=$monthToCompute AND YEAR(NOW())=YEAR(session.Date) GROUP BY user_session.UId HAVING s>3600 ORDER BY s DESC;";
	$result=exec_game_db_requete($requete);
	$ucount = 0;
	while($line = mysql_fetch_array($result))
	{
		printf("%s %s<br>\n",timeInSeconds2HumanDuration($line[0]),$line[1]);
		$ucount++;
	}
	//$totalUCount = totalRegisteredPlayer();
	printf("totoal = %s users of %s total user(%.0f%%)<br>\n",$ucount,$totalUCount,100*$ucount/$totalUCount);








	fclose($html_fp);	  
	include($cacheFileName);	
?>
