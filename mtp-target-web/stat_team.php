<?php
include_once("stat_function.php");
include_once("stat_game.php");

	$team = "unkown";
	if(isset($p_team) && validInput($p_team))
		$team = $p_team;
	else
		exit();

	$cacheFileName = $cache_dir."/stat_team_".$team.".html";
	
	if(isCacheFileUpToDate($cacheFileName))
	{
		include($cacheFileName);
		return;
	}
	
	$html_fp = fopen($cacheFileName, "wt");
	
	fprintf($html_fp,"<center><b>%s</b> team stats</center><br><br>",$team);
	
	fprintf($html_fp,"<table border=\"0\" cellpadding=\"0\" cellspacing=\"3\">\n");

	fprintf($html_fp,"members : <br><br>\n");
	$requete = "SELECT user.Login,user.UId,user.Score FROM user WHERE user.Login LIKE '[$team]%' ORDER BY user.Score DESC;";
	$result=exec_game_db_requete($requete);
	$totalScore = 0;
	while($line = mysql_fetch_array($result))
	{
		fprintf($html_fp,"<tr>\n");
		fprintf($html_fp,"<td>\n");
		fprintf($html_fp,"<a href=\"?page=stat_user.php&p_user_id=%d\">%s</a>&nbsp&nbsp&nbsp\n",$line[1],$line[0]);
		fprintf($html_fp,"</td>\n");
		fprintf($html_fp,"<td>\n");
		fprintf($html_fp,"%s\n",hugeNumber2Human($line[2]));
		fprintf($html_fp,"</td>\n");
		fprintf($html_fp,"</tr>\n");
		$totalScore += $line[2];
	}

	fprintf($html_fp,"</table>\n");
	fprintf($html_fp,"<br>\n");
	fprintf($html_fp,"Total score = %s\n",hugeNumber2Human($totalScore));

	fclose($html_fp);	  
	include($cacheFileName);	
?>
