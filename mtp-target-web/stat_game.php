<?php
include_once("stat_function.php");
include_once("stat_graph_display.php");


function drawGames($html_fp,$gameListRequete,$header,$uid)
{
	global $table_news_bgcolor_color;
	global $table_news_head_color;
	global $table_news_row_color;
	global $table_news_row_color_2;

	$maxPlayerCount = 16;
	fprintf($html_fp,"<table width=\"\" border=\"0\" cellpadding=\"0\" cellspacing=\"0\"  bgcolor=\"%s\">\n",$table_news_bgcolor_color);
	fprintf($html_fp,"<th bgcolor=\"%s\" align=\"center\" >\n",$table_news_head_color);
	fprintf($html_fp,"%s\n",$header);
	fprintf($html_fp,"</th>\n");
	fprintf($html_fp,"<tr>\n");
	fprintf($html_fp,"<td>\n");
	
	//$requete = "SELECT Id FROM session WHERE Duration>0 ORDER BY Id DESC LIMIT 0,$sessionCount";
	//$requete = "SELECT session.Id FROM session,user_session WHERE user_session.UId=$uid AND session.Id=user_session.SessionId AND session.Duration>0 ORDER BY user_session.Id DESC LIMIT 0,$sessionCount";
	$requete = $gameListRequete;//
	$sessionListResult  = exec_game_db_requete($requete);

	fprintf($html_fp,"<div id=\"stat_table\">");
	fprintf($html_fp,"<table width=\"100%%\" border=\"0\" cellpadding=\"0\" cellspacing=\"1\" >\n");
	//fprintf($html_fp,"<th> SId </th>\n");
	fprintf($html_fp,"<th> level </th>\n");
	fprintf($html_fp,"<th> server </th>\n");
	fprintf($html_fp,"<th> date </th>\n");
	fprintf($html_fp,"<th> duration </th>\n");
	fprintf($html_fp,"<th> score </th>\n");
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
			//fprintf($html_fp,"<td bgcolor=\"%s\">%s</td>\n",$table_news_row_color,$sessionListLine[0]);
			fprintf($html_fp,"<td bgcolor=\"%s\"><a href=\"?page=stat_map.php&p_map_id=%d\">%s</a></td>\n",$table_news_row_color,$sessionLine[7],$sessionLine[3]);
			fprintf($html_fp,"<td bgcolor=\"%s\"><a href=\"?page=stat_server.php&p_server_id=%d\">%s</a></td>\n",$table_news_row_color,$sessionLine[2],$sessionLine[6]);
			fprintf($html_fp,"<td bgcolor=\"%s\">%s</td>\n",$table_news_row_color,$sessionLine[1]);
			$duration = $sessionLine[5];
			if($duration<0)
				$duration = 0;
			fprintf($html_fp,"<td bgcolor=\"%s\">%s</td>\n",$table_news_row_color,$duration);
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
			fprintf($html_fp,"<td bgcolor=\"%s\">&nbsp %d &nbsp</td>\n",$table_news_row_color,$totalScore);

			fprintf($html_fp,"<td bgcolor=\"%s\">\n",$table_news_row_color);
			fprintf($html_fp,"<table  width=\"100%%\" border=\"0\" cellpadding=\"0\" cellspacing=\"0\">\n");
			
			fprintf($html_fp,"<tr class=\"expandable\">\n");
			for($i=0;$i<$userCount;$i++)
			{
				if((($i+$line_number)%2)==0)
					$cellBgColor = $table_news_row_color_2;
				else
					$cellBgColor = $table_news_row_color;
				fprintf($html_fp,"<td bgcolor=\"%s\">\n",$cellBgColor);
				fprintf($html_fp,"&nbsp %d &nbsp\n",$valPerUser[$i][2]);
				fprintf($html_fp,"</td>\n");
				$j++;
			}
			fprintf($html_fp,"</tr>\n");
			

			fprintf($html_fp,"<tr>\n");
			//while($sessionPlayerListLine = mysql_fetch_array($sessionPlayerListResult))
			for($i=0;$i<$userCount;$i++)
			{
				if((($i+$line_number)%2)==0)
					$cellBgColor = $table_news_row_color_2;
				else
					$cellBgColor = $table_news_row_color;
				fprintf($html_fp,"<td bgcolor=\"%s\">\n",$cellBgColor);
				if($uid!=$valPerUser[$i][0])
					fprintf($html_fp,"&nbsp<a href=\"?page=stat_user.php&p_user_id=%d\">%s</a>\n",$valPerUser[$i][0],$valPerUser[$i][1],$valPerUser[$i][2]);
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
	fprintf($html_fp,"</div>");
	
	fprintf($html_fp,"</td>\n");
	fprintf($html_fp,"</tr>\n");
	fprintf($html_fp,"</table>\n");
	fprintf($html_fp,"<br>\n");
	
}

function drawTopPlayedUser($html_fp,$uid)
{
	global $table_news_bgcolor_color;
	global $table_news_head_color;
	global $table_news_row_color;
	global $table_news_row_color_2;
	
	$header = "Top 10 played players last week";

	$requete = "SELECT user.Login,COUNT(t1.UId) as c ,t1.UId FROM session as s LEFT JOIN user_session as t1 ON t1.SessionId=s.Id LEFT JOIN user_session as t2 ON t2.SessionId=s.Id LEFT JOIN user ON user.UId=t1.UId WHERE TO_DAYS(NOW())-TO_DAYS(s.Date)<7 AND t2.UId=$uid AND t1.UId!=$uid GROUP BY t1.UId ORDER BY c DESC LIMIT 0,10;";
	//$requete = "SELECT user.Login,COUNT(t1.UId) as c FROM session as s LEFT JOIN user_session as t1 ON t1.SessionId=s.Id LEFT JOIN user_session as t2 ON t2.SessionId=s.Id LEFT JOIN user ON user.UId=t1.UId WHERE s.Id<100 AND t2.UId=$uid AND t1.UId!=$uid GROUP BY t1.UId ORDER BY c DESC LIMIT 0,10;";
	$result=exec_game_db_requete($requete);


	fprintf($html_fp,"<table width=\"\" border=\"0\" cellpadding=\"0\" cellspacing=\"0\"  bgcolor=\"%s\">\n",$table_news_bgcolor_color);
	fprintf($html_fp,"<th bgcolor=\"%s\" align=\"center\" >\n",$table_news_head_color);
	fprintf($html_fp,"%s\n",$header);
	fprintf($html_fp,"</th>\n");
	fprintf($html_fp,"<tr>\n");
	fprintf($html_fp,"<td>\n");
	
	fprintf($html_fp,"<div id=\"stat_table\">");
	fprintf($html_fp,"<table width=\"100%%\" border=\"0\" cellpadding=\"0\" cellspacing=\"1\" >\n");
	fprintf($html_fp,"<th> user </th>\n");
	fprintf($html_fp,"<th> games played with him </th>\n");
	
	$line_number = 0;
	while($line = mysql_fetch_array($result))
	{
		fprintf($html_fp,"<tr height=\"100%%\" >\n");
		fprintf($html_fp,"<td bgcolor=\"%s\">\n",$table_news_row_color);
		fprintf($html_fp,"<a href=\"?page=stat_user.php&p_user_id=%d\">%s</a><br>\n",$line[2],$line[0]);
		fprintf($html_fp,"</td>\n");
		fprintf($html_fp,"<td bgcolor=\"%s\">%s</td>\n",$table_news_row_color,$line[1]);
		fprintf($html_fp,"</tr>\n");
		$line_number++;
	}
	if($line_number==0)
	{
		fprintf($html_fp,"<tr height=\"100%%\" >\n");
		fprintf($html_fp,"<td colspan=\"2\" bgcolor=\"%s\">none</td>\n",$table_news_row_color);
		fprintf($html_fp,"</tr>\n");
	}
	
	fprintf($html_fp,"</table>\n");
	fprintf($html_fp,"</div>");
	
	fprintf($html_fp,"</td>\n");
	fprintf($html_fp,"</tr>\n");
	fprintf($html_fp,"</table>\n");
	fprintf($html_fp,"<br>\n");

}


?>