<?php 
	function getStats(&$NbOnlinePlayers, &$NbRegisteredPlayers, &$NbSessions)
	{
		$DBHost = "mtp-target.dyndns.org";
		$DBUserName = "www";
		$DBPassword = "www";
		$DBName = "nel";

		$link = mysql_connect($DBHost, $DBUserName, $DBPassword) or die ("Can't connect to database host:$DBHost user:$DBUserName");
		mysql_select_db ($DBName) or die ("Can't access to the table dbname:$DBName");
		$query = "select count(*) from user where State='Online'";
		$result = mysql_query ($query) or die ("Can't execute the query: ".$query);
		$row = mysql_fetch_row ($result);
		$NbOnlinePlayers = $row[0];

		$query = "select count(*) from user";
		$result = mysql_query ($query) or die ("Can't execute the query: ".$query);
		$row = mysql_fetch_row ($result);
		$NbRegisteredPlayers = $row[0];

		$query = "select count(*) from session";
		$result = mysql_query ($query) or die ("Can't execute the query: ".$query);
		$row = mysql_fetch_row ($result);
		$NbSessions = $row[0];
	}
?>
