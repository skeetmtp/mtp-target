<?php

function auto_bench_start($id)
{
	global $auto_bench_time_start;
	$auto_bench_time_start[$id] = microtime_float();
}

function auto_bench_end($id)
{
	global $auto_bench_time_start;
	$bench_time_end = microtime_float();
	$bench_time = $bench_time_end - $auto_bench_time_start[$id];
	printf("bench test '%s' done in %.2f seconds<br>\n",$id,$bench_time);
}

function bench_start()
{
	global $bench_time_start;
	$bench_time_start = microtime_float();
}

function bench_end()
{
	global $bench_time_start;
	$bench_time_end = microtime_float();
	$bench_time = $bench_time_end - $bench_time_start;
	return sprintf("%.2f",$bench_time);
}


function microtime_float()
{
   list($usec, $sec) = explode(" ", microtime());
   return ((float)$usec + (float)$sec);
} 

function assertValidNumber($input)
{
	if(validNumber($input))
		return;
	else
    	die("input error : php stopped");
}

function validNumber($input)
{
  $res = (ereg("^[0-9]+$",$input));
  return $res;
}

function validInput($input)
{
 	if(strlen($input)>20) return false;
		$res = (ereg("^[\-_\.A-Za-z0-9([)(|\]|])]+$",$input));
  //$res = (ereg("^[_\.A-Za-z0-9\-]+$",$input));
  /*
  if(!$res)
  	echo "not a valid input : ".$input."<br>";
  */
  return $res;
}

function validPage($page)
{
	if(strncmp($page,"/",1)==0)	return false;
	if(strstr($page,".."))	return false;
  	if(strlen($page)>128) return false;
    if(!ereg("^[_/\.A-Za-z0-9\-]+$",$page))
    	return false;
    return file_exists($page);
}

function checkLoginPassword($login,$password)
{
	$requete = "SELECT * FROM user WHERE Login='".$login."';";
	$resultat = exec_game_db_requete($requete);

	if ($ligne = mysql_fetch_array($resultat)) 
	{
		$db_login = $ligne[1];
		$db_password_crypted = $ligne[2];
		if(crypt($password, $db_password_crypted) == $db_password_crypted)
			return true;
	}
	return false; 
}

function checkLoginPasswordCookie(&$login,&$password)
{
global $user_id;
	$requete = "SELECT * FROM game_user WHERE Login='".$login."';";
	$resultat = exec_requete($requete);

	$res = false;
	$user_found = false;
	if ($ligne = mysql_fetch_array($resultat)) 
	{
		//echo "user found in db<br>";
		$login = $ligne[1];
		$user_crypted = $ligne[2];
		$user_id = userName2Uid($login);
		setcookie("mtp_target_user_id", $user_id, time() + 3600 * 24 * 30);
		setcookie("mtp_target_user_login", $login, time() + 3600 * 24 * 30);
		setcookie("mtp_target_user_password", $password, time() + 3600 * 24 * 30);
		/*
		echo "user1  : ".$login;
		echo "<br>";
		echo "pass  : ".$password;
		echo "<br>";
		echo "crypt : ".$user_crypted;
		echo "<br>";
		*/
		
		$res = crypt($password, $user_crypted) == $user_crypted;
		
		if($res)
		{
			$requete = sprintf("UPDATE game_user set LastLoginDate=NOW() WHERE UId='%s';",$user_id,$login,$user_crypted);
			$resultat = exec_requete($requete);
		}
		
		$user_found = true;
	}
	if($res)
	{
		return true;
	}
	
	$requete = "SELECT * FROM user WHERE Login='".$login."';";
	$resultat = exec_game_db_requete($requete);
	
	if ($ligne = mysql_fetch_array($resultat)) 
	{
		//echo "user found in db<br>";
		$login = $ligne[1];
		$user_crypted = $ligne[2];
		$user_id = userName2Uid($login);
		setcookie("mtp_target_user_id", $user_id, time() + 3600 * 24 * 30);
		setcookie("mtp_target_user_login", $login, time() + 3600 * 24 * 30);
		setcookie("mtp_target_user_password", $password, time() + 3600 * 24 * 30);
		/*
		echo "user2  : ".$login;
		echo "<br>";
		echo "pass  : ".$password;
		echo "<br>";
		echo "crypt : ".$user_crypted;
		echo "<br>";
		*/
		if(crypt($password, $user_crypted) == $user_crypted)
		{
			if(!$user_found)
				$requete = sprintf("INSERT INTO game_user (UId,Login,Password) VALUES ('%s','%s','%s');",$user_id,$login,$user_crypted);
			else
				//$requete = sprintf("UPDATE game_user set UId='%s', Login='%s', Password='%s';",$user_id,$login,$user_crypted);
				$requete = sprintf("UPDATE game_user set UId='%s', Login='%s', Password='%s', LastLoginDate=NOW();",$user_id,$login,$user_crypted);
			$resultat = exec_requete($requete);
			return true;	
		}
		else
			return false;
	}
	else 
	{
		unset ($login);
		unset ($passwor	);
		unset ($user_crypted);
		return false;
	}
}




?>
