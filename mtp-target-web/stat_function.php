<?php
include_once("stat_graph_display.php");

function Id2ServerName($server_id)
{
	  $requete = "SELECT Name FROM shard WHERE ShardId='".$server_id."';";
	  $resultat=exec_game_db_requete($requete);
	
	  if($ligne = mysql_fetch_array($resultat))
	  {
	  	return $ligne[0];
	  }
	  echo "server not found : ".$server_id;
	  exit;
	  return -1;	
}

function Id2TextureName($texture_id)
{
	  $requete = "SELECT Name FROM texture WHERE Id='".$texture_id."';";
	  $resultat=exec_game_db_requete($requete);
	
	  if($ligne = mysql_fetch_array($resultat))
	  {
	  	return $ligne[0];
	  }
	  echo "texture not found : ".$texture_id;
	  exit;
	  return -1;	
}

function textureName2Id($texture_name)
{
	  $requete = "SELECT Id FROM texture WHERE Name='".$texture_name."';";
	  $resultat=exec_game_db_requete($requete);
	
	  if($ligne = mysql_fetch_array($resultat))
	  {
	  	return $ligne[0];
	  }
	  echo "texture not found : ".$texture_name;
	  exit;
	  return -1;	
}

function userNameExist($user_name)
{
	  $requete = "SELECT UId FROM user WHERE Login='".$user_name."';";
	  $resultat=exec_game_db_requete($requete);
	
	  if($ligne = mysql_fetch_array($resultat))
	  {
	  	return true;
	  }
	  return false;	
}

function userName2Uid($user_name)
{
	  $requete = "SELECT UId FROM user WHERE Login='".$user_name."';";
	  $resultat=exec_game_db_requete($requete);
	
	  if($ligne = mysql_fetch_array($resultat))
	  {
	  	return $ligne[0];
	  }
	  echo "user name not found : ".$user_name;
	  exit;
	  return -1;	
}

function userId2Name($user_id)
{
	  $requete = "SELECT Login FROM user WHERE UId=".$user_id.";";
	  $resultat=exec_game_db_requete($requete);
	
	  if($ligne = mysql_fetch_array($resultat))
	  {
	  	return $ligne[0];
	  }
	  echo "user id not found : ".$user_id;
	  exit;
	  return -1;	
}

function mapId2Name($map_id)
{
	  $requete = "SELECT LevelName FROM map WHERE Id=".$map_id.";";
	  $resultat=exec_game_db_requete($requete);
	
	  if($ligne = mysql_fetch_array($resultat))
	  {
	  	return $ligne[0];
	  }
	  echo "map id not found : ".$map_id;
	  exit;
	  return -1;	
}

function userTotalSessionPlayedCount($uid)
{
	$requete = sprintf("SELECT COUNT(*) FROM user_session WHERE UId='%d';",$uid);
	$resultat=exec_game_db_requete($requete);
	if($ligne = mysql_fetch_array($resultat))
	{
		return $ligne[0];
	}
	else return 0;
	
}

function userTotalSessionWithPointCount($uid)
{
	$requete = sprintf("SELECT COUNT(*) FROM user_session WHERE UId='%d' AND Duration>0 AND Score>0;",$uid);
	$resultat=exec_game_db_requete($requete);
	if($ligne = mysql_fetch_array($resultat))
	{
		return $ligne[0];
	}
	else return 0;
	
}

function userTotalSessionPlayedDuration($uid)
{
	$requete = sprintf("SELECT SUM(Duration) as totalDuration FROM user_session WHERE UId='%d' AND Duration>0;",$uid);
	$resultat=exec_game_db_requete($requete);
	if($ligne = mysql_fetch_array($resultat))
	{
		return $ligne[0];
	}
	else return 0;
	
}

function userTotalScore($uid)
{
	$requete = sprintf("SELECT SUM(Score) as totalScore FROM user_session WHERE UId='%d' AND Duration>0;",$uid);
	$resultat=exec_game_db_requete($requete);
	if($ligne = mysql_fetch_array($resultat))
	{
		return $ligne[0];
	}
	else return 0;
	
}

function userRank($score)
{
	$requete = sprintf("SELECT COUNT(*) FROM user WHERE Score>%d;",$score);
	$resultat=exec_game_db_requete($requete);
	if($ligne = mysql_fetch_array($resultat))
	{
		return $ligne[0]+1;
	}
	else return 0;
	
}

function userCurrentScore($uid)
{
	$requete = sprintf("SELECT Score FROM user WHERE UId='%d';",$uid);
	$resultat=exec_game_db_requete($requete);
	if($ligne = mysql_fetch_array($resultat))
	{
		return $ligne[0];
	}
	else return 0;
	
}

function userTexture($uid)
{
	$requete = sprintf("SELECT Texture FROM user WHERE UId='%d';",$uid);
	$resultat=exec_game_db_requete($requete);
	if($ligne = mysql_fetch_array($resultat))
	{
		if($ligne[0]=="")
			return "blue";
		else
			return $ligne[0];
	}
	else 
		return "blue";
	
}

function userOverallStat($uid,&$totalScore,&$currentScore,&$totalSession,&$sessionPlayedDuration)
{
	$requete = sprintf("SELECT user.Score,user.Registered FROM user WHERE UId='%d';",$uid);
	$resultat=exec_game_db_requete($requete);
	if($ligne = mysql_fetch_array($resultat))
	{
		$totalScore = $ligne[0];
		$registrationDate = $ligne[1];
	}
	$requete = sprintf("SELECT SUM(Score), COUNT(*), SUM(Duration) FROM user_session WHERE UId='%d';",$uid);
	$resultat=exec_game_db_requete($requete);
	if($ligne = mysql_fetch_array($resultat))
	{
		$currentScore = $ligne[0];
		$totalSession = $ligne[1];
		$sessionPlayedDuration = $ligne[2];
	}
}


function hugeNumber2HumanDuration($score)
{
	$res = "";
	$strScore = "".$score;
	
	$l = strlen($strScore);
	if($l==0)
		return "";
	
	$j=0;
	for($i=$l-1;$i>=0;$i--)
	{
		if(($j%3)==0 && $j!=0)
		{
		  	$res = "," . $res;
		}
	  	$res = $strScore[$i] . $res;
		$j++;
	}
	return $res;
	
}

function timeInSeconds2HumanDuration($t3)
{
	/*
	$t1 = strtotime(date("Y-m-d H:i:s"));
	$t2 = strtotime(date("Y-m-d H:i:s",$lastRestartTime));
	$t3 = $t1-$t2;
	*/
	
	$upSec = $t3%60;
	$upMin = ($t3/60)%60;
	$upHour = (($t3/60)/60)%24;
	$upDay = ((($t3/60)/60)/24)%100;

	if($upDay == 1)
		return sprintf("%d day %02d:%02d:%02d",$upDay,$upHour,$upMin,$upSec);
	else if($upDay > 1)
		return sprintf("%d days %02d:%02d:%02d",$upDay,$upHour,$upMin,$upSec);
	else
		return sprintf("%02d:%02d:%02d",$upHour,$upMin,$upSec);
	
}

function userLastSessionDate($uid)
{
	$requete = "SELECT session.Date FROM session,user_session WHERE session.Id=user_session.SessionId AND user_session.UId=$uid ORDER BY session.Date DESC LIMIT 0,1 ;";
	$result=exec_game_db_requete($requete);
	if($ligne = mysql_fetch_array($result))
	{
		return $ligne[0];
	}
	return "Unknown";
}

function userFirstSessionDate($uid)
{
	$requete = "SELECT session.Date FROM session,user_session WHERE session.Id=user_session.SessionId AND user_session.UId=$uid ORDER BY session.Date LIMIT 0,1 ;";
	$result=exec_game_db_requete($requete);
	if($ligne = mysql_fetch_array($result))
	{
		return $ligne[0];
	}
	return "Unknown";
}

function userTodayScore($uid)
{
	$requete = "SELECT SUM(user_session.Score) as s FROM session,user_session WHERE session.Id=user_session.SessionId AND user_session.UId=$uid AND TO_DAYS(NOW())-TO_DAYS(session.Date)=0 GROUP BY user_session.UId ORDER BY s DESC LIMIT 0,1 ;";
	$result=exec_game_db_requete($requete);
	if($ligne = mysql_fetch_array($result))
	{
		return $ligne[0];
	}
	return "0";
}

function userRegistrationDateInDays($uid)
{
	$requete = "SELECT TO_DAYS(NOW())-TO_DAYS(user.Registered) FROM user WHERE user.UId=$uid LIMIT 0,1 ;";
	$result=exec_game_db_requete($requete);
	if($ligne = mysql_fetch_array($result))
	{
		return $ligne[0];
	}
	return "Unknown";
}

function totalRegisteredPlayer()
{
	$query = "SELECT COUNT(*) FROM user";
	$result = exec_game_db_requete($query);
	if($ligne = mysql_fetch_array($result))
	{
		return $ligne[0];
	}
	return 0;
}


?>