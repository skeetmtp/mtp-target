<?php
include_once("stat_function.php");
include_once("stat_game.php");


function updateMapListDb($silent)
{
	if(!$silent)
		printf("Updating Map List : <br>\n");
	$requete = "SELECT DISTINCT LevelName FROM session;";
	$resultMapList=exec_game_db_requete($requete);
	while($mapList = mysql_fetch_array($resultMapList))
	{
		$requete = sprintf("SELECT Id FROM map WHERE LevelName='%s' LIMIT 0,1;",$mapList[0]);
		$result=exec_game_db_requete($requete);
		if(!mysql_fetch_array($result))
		{
			$requete = sprintf("INSERT INTO map (LevelName) VALUES ('%s');",$mapList[0]);
			$result=exec_game_db_requete($requete);
		}
	}
	
	if(!$silent)
	{
		$requete = "SELECT LevelName FROM map;";
		$resultMapList=exec_game_db_requete($requete);
		while($mapList = mysql_fetch_array($resultMapList))
		{
				printf("%s<br>\n",$mapList[0]);
		}
	}
}

	updateMapListDb(false);
	
?>