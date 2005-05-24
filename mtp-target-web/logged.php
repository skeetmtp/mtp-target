<?php

	require_once("ingame_stats.php");

	echo "<table class=\"stat\">";
	printf("<th>Server</th>");
	printf("<th>Login</th>");
	printf("<th>Score</th>");
	$result = exec_game_db_requete("select * from user WHERE State='Online' ORDER BY ShardId DESC, Score DESC");
	while ($line = mysql_fetch_array($result))
	{
		$sid_res = exec_game_db_requete("select * from shard WHERE ShardId=".$line[3]);
		$sid_array = mysql_fetch_row ($sid_res);
		printf("<tr>");
		printf("<td>%s</td>",$sid_array[3]);
		printf("<td><a href=\"?page=stat_user&p_user_id=%d\">%s</a></td>",$line[0],$line[1]);
		printf("<td>%s</td>",$line[6]);
		printf("</tr>");
	}	
	printf("</table>");

?>
