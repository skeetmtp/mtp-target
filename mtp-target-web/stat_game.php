<?php
require_once("stat_function.php");
require_once("stat_graph_display.php");


function drawGames($html_fp,$gameListRequete,$header,$uid)
{
	$maxPlayerCount = 16;
	fprintf($html_fp,"<table class=\"stat\">\n");
	fprintf($html_fp,"<th>%s</th>\n",$header);
	fprintf($html_fp,"<tr>\n");
	fprintf($html_fp,"<td>\n");
	
	//$requete = "SELECT Id FROM session WHERE Duration>0 ORDER BY Id DESC LIMIT 0,$sessionCount";
	//$requete = "SELECT session.Id FROM session,user_session WHERE user_session.UId=$uid AND session.Id=user_session.SessionId AND session.Duration>0 ORDER BY user_session.Id DESC LIMIT 0,$sessionCount";
	$requete = $gameListRequete;//
	$sessionListResult  = exec_game_db_requete($requete);

//	fprintf($html_fp,"<div id=\"stat_table\">");
	fprintf($html_fp,"<table>\n");
	//fprintf($html_fp,"<th> SId </th>\n");
	fprintf($html_fp,"<th>level</th>\n");
	fprintf($html_fp,"<th>server</th>\n");
	fprintf($html_fp,"<th>date</th>\n");
	fprintf($html_fp,"<th>duration</th>\n");
	fprintf($html_fp,"<th>score</th>\n");
	fprintf($html_fp,"<th colspan=\"%d\"> players </th>\n",$maxPlayerCount);
	
	$line_number = 0;
	while($sessionListLine = mysql_fetch_array($sessionListResult))
	{
		$SId = $sessionListLine[0];
		$requete = "SELECT session.*,shard.Name,map.Id FROM session,shard,map WHERE session.Id=$sessionListLine[0] AND shard.ShardId=session.SId AND map.LevelName=session.LevelName LIMIT 0,1";
		$sessionResult  = exec_game_db_requete($requete);
		if($sessionLine = mysql_fetch_array($sessionResult))
		{			
			fprintf($html_fp,"<tr height=\"100%%\" >\n");
			fprintf($html_fp,"<td><a href=\"?page=stat_map&p_map_id=%d\">%s</a></td>\n",$sessionLine[7],$sessionLine[3]);
			fprintf($html_fp,"<td><a href=\"?page=stat_server&p_server_id=%d\">%s</a></td>\n",$sessionLine[2],$sessionLine[6]);
			fprintf($html_fp,"<td>%s</td>\n",$sessionLine[1]);
			$duration = $sessionLine[5];
			if($duration<0)
				$duration = 0;
			fprintf($html_fp,"<td>%s</td>\n",$duration);
			$requete = "SELECT user.UId,user.Login,user_session.Score FROM user,user_session WHERE user_session.SessionId=$sessionListLine[0] AND user.UId=user_session.UId";
			//$requete = "SELECT user.UId,user.Login,user_session.Score FROM user LEFT JOIN user_session ON user.UId=user_session.UId WHERE user_session.SessionId=$sessionListLine[0]";
			$sessionPlayerListResult  = exec_game_db_requete($requete);
			$j = 0;
	
			$totalScore = 0;
			$i = 0;
			while($sessionPlayerListLine = mysql_fetch_array($sessionPlayerListResult))
			{
				$valPerUser[$i][0]=$sessionPlayerListLine[0];
				$valPerUser[$i][1]=$sessionPlayerListLine[1];
				$valPerUser[$i][2]=$sessionPlayerListLine[2];
				$totalScore += $sessionPlayerListLine[2];
				$i++;
			}
			$userCount = $i;
			fprintf($html_fp,"<td>&nbsp %d &nbsp</td>\n",$totalScore);

			fprintf($html_fp,"<td>\n");
			fprintf($html_fp,"<table  width=\"100%%\" border=\"0\" cellpadding=\"0\" cellspacing=\"0\">\n");
			
			fprintf($html_fp,"<tr class=\"expandable\">\n");
			for($i=0;$i<$userCount;$i++)
			{
				fprintf($html_fp,"<td>\n");
				fprintf($html_fp,"%d\n",$valPerUser[$i][2]);
				fprintf($html_fp,"</td>\n");
				$j++;
			}
			fprintf($html_fp,"</tr>\n");
			

			fprintf($html_fp,"<tr>\n");
			//while($sessionPlayerListLine = mysql_fetch_array($sessionPlayerListResult))
			for($i=0;$i<$userCount;$i++)
			{
				fprintf($html_fp,"<td>\n");
				if($uid!=$valPerUser[$i][0])
					fprintf($html_fp,"&nbsp<a href=\"?page=stat_user&p_user_id=%d\">%s</a>\n",$valPerUser[$i][0],$valPerUser[$i][1],$valPerUser[$i][2]);
				else	
					fprintf($html_fp,"&nbsp %s\n",$valPerUser[$i][1]);
				fprintf($html_fp,"</td>\n");
				$j++;
			}
			fprintf($html_fp,"</tr>\n");
			fprintf($html_fp,"</table>\n");
			
			fprintf($html_fp,"</td>\n");
	
			fprintf($html_fp,"</tr>\n");
		}
		$line_number++;
	}
	
	fprintf($html_fp,"</table>\n");
//	fprintf($html_fp,"</div>");
	
	fprintf($html_fp,"</td>\n");
	fprintf($html_fp,"</tr>\n");
	fprintf($html_fp,"</table>\n");
	fprintf($html_fp,"<br/>\n");
	
}

function drawTopPlayedUser($html_fp,$uid)
{
	$header = "Top 10 played players last week";

	$requete = "SELECT user.Login,COUNT(t1.UId) as c ,t1.UId FROM session as s LEFT JOIN user_session as t1 ON t1.SessionId=s.Id LEFT JOIN user_session as t2 ON t2.SessionId=s.Id LEFT JOIN user ON user.UId=t1.UId WHERE TO_DAYS(NOW())-TO_DAYS(s.Date)<7 AND t2.UId=$uid AND t1.UId!=$uid GROUP BY t1.UId ORDER BY c DESC LIMIT 0,10;";
	//$requete = "SELECT user.Login,COUNT(t1.UId) as c FROM session as s LEFT JOIN user_session as t1 ON t1.SessionId=s.Id LEFT JOIN user_session as t2 ON t2.SessionId=s.Id LEFT JOIN user ON user.UId=t1.UId WHERE s.Id<100 AND t2.UId=$uid AND t1.UId!=$uid GROUP BY t1.UId ORDER BY c DESC LIMIT 0,10;";
	$result=exec_game_db_requete($requete);


	fprintf($html_fp,"<table class=\"stat\">\n");
	fprintf($html_fp,"<th>\n");
	fprintf($html_fp,"%s\n",$header);
	fprintf($html_fp,"</th>\n");
	fprintf($html_fp,"<tr>\n");
	fprintf($html_fp,"<td>\n");
	
	fprintf($html_fp,"<div id=\"stat_table\">");
	fprintf($html_fp,"<table>\n");
	fprintf($html_fp,"<th>user</th>\n");
	fprintf($html_fp,"<th>games played with him</th>\n");
	
	$line_number = 0;
	while($line = mysql_fetch_array($result))
	{
		fprintf($html_fp,"<tr>\n");
		fprintf($html_fp,"<td>\n");
		fprintf($html_fp,"<a href=\"?page=stat_user&p_user_id=%d\">%s</a><br/>\n",$line[2],$line[0]);
		fprintf($html_fp,"</td>\n");
		fprintf($html_fp,"<td>%s</td>\n",$line[1]);
		fprintf($html_fp,"</tr>\n");
		$line_number++;
	}
	if($line_number==0)
	{
		fprintf($html_fp,"<tr>\n");
		fprintf($html_fp,"<td>none</td>\n");
		fprintf($html_fp,"</tr>\n");
	}
	
	fprintf($html_fp,"</table>\n");
	fprintf($html_fp,"</div>");
	
	fprintf($html_fp,"</td>\n");
	fprintf($html_fp,"</tr>\n");
	fprintf($html_fp,"</table>\n");
	fprintf($html_fp,"<br/>\n");

}

?>
