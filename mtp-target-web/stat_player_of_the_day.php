<?php
require_once("stat_function.php");
require_once("stat_game.php");
require_once("helpers.php");

	$cacheFileName = $cache_dir."/stat_player_of_the_day_".CUser::instance()->language().".html";
	boxBegin(true);

	if(isCacheFileUpToDateDuration($cacheFileName,$defaultScoresCacheFileDuration))
	{
		require_once($cacheFileName);
	}
	else if($enableSql)
	{
		$html_fp = fopen($cacheFileName, "wt");

		$playerTopCount = 3;
		$requete = sprintf("SELECT user.Login,SUM(user_session.Score) as s,user.UId FROM session,user_session,user WHERE TO_DAYS(NOW())-TO_DAYS(session.Date)=0 AND session.Id=user_session.SessionId AND user_session.UId=user.UId GROUP BY user_session.UId ORDER BY s DESC LIMIT 0,%d;",$playerTopCount);
		$result=exec_game_db_requete($requete);
		if($line = mysql_fetch_array($result))
		{
			fprintf($html_fp,"<center>");
			fprintf($html_fp,"%s :<br/>\n", lg('BestPlayersOfTheDay'));
			fprintf($html_fp,"<a href=\"?page=stat_user&p_user_id=%d\">%s</a>: %s %s<br/>\n",$line[2],$line[0],$line[1], lg('Points'));
			fprintf($html_fp,"</center>");
			fprintf($html_fp,"<br/>");
		}

		fprintf($html_fp,"<div id=\"small\">");
		$nth = 2;
		while($line = mysql_fetch_array($result))
		{
			fprintf($html_fp,"#%d <a href=\"?page=stat_user&p_user_id=%d\">%s</a>: %s %s<br/>\n",$nth,$line[2],$line[0],$line[1], lg('Points'));
			$nth++;
		}

		$requete = "SELECT * FROM user ORDER BY UId DESC LIMIT 0,1 ;";
		$result=exec_game_db_requete($requete);
		if($line = mysql_fetch_array($result))
		{
			fprintf($html_fp,"%s: <a href=\"?page=stat_user&p_user_id=%d\">%s</a>", lg('LastestRegistered'), $line[0],$line[1]);
			fprintf($html_fp,"<br/>\n");
		}
	
		fprintf($html_fp,"</div>");
		fclose($html_fp);	  
		require_once($cacheFileName);
	}
	
	/*
	$requete = "SELECT COUNT(*) FROM game_user WHERE UNIX_TIMESTAMP(NOW())-UNIX_TIMESTAMP(LastLoginDate)<$webUserIdleTime;";
	$result=exec_requete($requete);
	if($line = mysql_fetch_array($result))
	{
		printf($potdUserOnWebCount,$line[0]);
		printf("<br/>");
	}
	*/

	boxEnd();
?>
