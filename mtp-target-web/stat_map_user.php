<?php
include_once("stat_function.php");
include_once("stat_game.php");

	assertValidNumber($p_user_id);
	assertValidNumber($p_map_id);
	$uid = $p_user_id;
	$map_id = $p_map_id;
	$user_name = userId2Name($uid);
	$mapName = mapId2Name($map_id);
	$cacheFileName = $cache_dir."/stat_map_user_".$uid."_".$map_id.".html";
	printf("<center><b>%s</b> stats on %s : </center><br><br>",$user_name,$mapName);

	if(isCacheFileUpToDate($cacheFileName))
	{
		include($cacheFileName);
		return;
	}
	$html_fp = fopen($cacheFileName, "wt");

	$totalScore = userTotalScore($uid);
	$currentScore = userCurrentScore($uid);
	$sessionPlayedCount = userTotalSessionPlayedCount($uid);
	$sessionPlayedDuration = userTotalSessionPlayedDuration($uid);
	$sessionWithScoreCount = userTotalSessionWithPointCount($uid);
	
	fprintf($html_fp,"<a href=\"?page=stat_map.php&p_map_id=%d\">%s<br>\n",$map_id,$mapName);
	fprintf($html_fp,"<img src=\"$sshot_dir/%s.jpg\" width=\"200\" ALT=\"%s screenshot \"></a><br>\n",$mapName,$mapName);
	$requete = sprintf("SELECT COUNT(*) , SUM(user_session.Score) , SUM(user_session.Duration)  FROM user_session,session WHERE user_session.UId=%d AND user_session.SessionId=session.Id AND session.LevelName='%s' AND user_session.Duration>0;",$uid,$mapName);
	$resultat=exec_game_db_requete($requete);
	
	//while($ligne = mysql_fetch_array($resultat))
	if($ligne = mysql_fetch_array($resultat))
	{
		$levelSessionCountRatio = $ligne[0]/$sessionPlayedCount;
		$levelScoreRatio = $ligne[1]/$totalScore;
		$levelRemtability = 0;
		if($levelSessionCountRatio>0)
			$levelRemtability = $levelScoreRatio/$levelSessionCountRatio;
		fprintf($html_fp,"%d games played on this level (%.0f%% of total games)<br>\n",$ligne[0],100*$levelSessionCountRatio);
		fprintf($html_fp,"level total score = %d (%.0f%% of total score)<br>\n",$ligne[1],100*$levelScoreRatio);
		if($ligne[0]>0)
			fprintf($html_fp,"about %.0f points per game<br>\n",$ligne[1]/$ligne[0]);
		fprintf($html_fp,"skill on this level = %.0f%%<br>\n",100*$levelRemtability);
		fprintf($html_fp,"(>100%% your good in this level, <100%% your not good in this level, 100%% just average skill)<br>\n");
		fprintf($html_fp,"total time spent on this level %s<br>\n",timeInSeconds2HumanDuration($ligne[2]));
		fprintf($html_fp,"<br>\n");
	}
	
	
	fclose($html_fp);	  
	
	include($cacheFileName);	
?>	