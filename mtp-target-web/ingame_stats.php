<?php 
	include_once("config.php");
	
	function getStats(&$NbOnlinePlayers, &$NbRegisteredPlayers, &$NbSessions)
	{
		$query = "select count(*) from user where State='Online'";
		$result = exec_game_db_requete($query);
		$row = mysql_fetch_row ($result);
		$NbOnlinePlayers = $row[0];

		$query = "select count(*) from user";
		$result = exec_game_db_requete($query);
		$row = mysql_fetch_row ($result);
		$NbRegisteredPlayers = $row[0];

		$query = "select count(*) from session";
		$result = exec_game_db_requete($query);
		$row = mysql_fetch_row ($result);
		$NbSessions = $row[0];
	}
	
	function getUserTotalDuration($uid,&$totalDurationStr)
	{
		$uid_res = exec_game_db_requete(sprintf("select SUM(Duration) from user_session WHERE UId=%d",$uid));
		$uid_array = mysql_fetch_row ($uid_res);
		$total_time = $uid_array[0];
		$total_sec = ($total_time)%60;
		$total_min = ($total_time/60)%60;
		$total_hour = ($total_time/60/60)%24;
		$totalDurationStr = sprintf("%02d:%02d:%02d",$total_hour,$total_min,$total_sec);
		return $total_time;
	}
	
?>
