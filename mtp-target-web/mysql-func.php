<?php

require_once('common.php');
/**
 * Quote a string to avoid SQL injection. Must be call for all parameters of a SQL request
 *
 * @param $value to quote
 * @return quoted value
 */
function smartQuote($value)
{
   // Stripslashes
   if (get_magic_quotes_gpc()) {
       $value = stripslashes($value);
   }
   // Protection if not a number
   
   if (!is_numeric($value)) {
       $value = "'" . mysql_real_escape_string($value) . "'";
   }

   return $value;
}


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
	printf("bench test '%s' done in %.2f seconds<br/>\n",$id,$bench_time);
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
	return $res;
}

function checkLoginPassword($login,$password)
{
	die("REMOVED BY ACE. OLD CODE ".__FILE__." ".__LINE__);
	return;
	
	$requete = "SELECT * FROM user WHERE Login='".$login."';";
	//$resultat = exec_game_db_requete($requete);
	$resultat = exec_requete($requete);

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
	die("REMOVED BY ACE. OLD CODE ".__FILE__." ".__LINE__);
	return;

	global $user_id;
	$requete = "SELECT * FROM game_user WHERE Login='".$login."';";
	$resultat = exec_requete($requete);

	$res = false;
	$user_found = false;
	if ($ligne = mysql_fetch_array($resultat)) 
	{
		//echo "user found in db<br/>";
		$login = $ligne[1];
		$user_crypted = $ligne[2];
		$user_id = userName2Uid($login);
		setcookie("mtp_target_user_id", $user_id, time() + 3600 * 24 * 30);
		setcookie("mtp_target_user_login", $login, time() + 3600 * 24 * 30);
		setcookie("mtp_target_user_password", $password, time() + 3600 * 24 * 30);
		/*
		echo "user1  : ".$login;
		echo "<br/>";
		echo "pass  : ".$password;
		echo "<br/>";
		echo "crypt : ".$user_crypted;
		echo "<br/>";
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
		//echo "user found in db<br/>";
		$login = $ligne[1];
		$user_crypted = $ligne[2];
		$user_id = userName2Uid($login);
		setcookie("mtp_target_user_id", $user_id, time() + 3600 * 24 * 30);
		setcookie("mtp_target_user_login", $login, time() + 3600 * 24 * 30);
		setcookie("mtp_target_user_password", $password, time() + 3600 * 24 * 30);
		/*
		echo "user2  : ".$login;
		echo "<br/>";
		echo "pass  : ".$password;
		echo "<br/>";
		echo "crypt : ".$user_crypted;
		echo "<br/>";
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
		unset ($password);
		unset ($user_crypted);
		return false;
	}
}

$var_bench = "";

function m_bench_start()
{
	return microtime_float();
}

function m_bench_end($m_bench_time_start)
{
	$m_bench_time_end = microtime_float();
	$m_bench_time = $m_bench_time_end - $m_bench_time_start;
	return sprintf("%.2f",$m_bench_time);
}

function exec_requete($requete)
{
	global $var_bench;
	global $MySqlLinkIdentifier1;
	global $ip1, $login1, $password1;
	
	$total = m_bench_start();

	$var_bench .= "'".$requete."'";
	
	if(!isset($MySqlLinkIdentifier1)) {
		$t1 = m_bench_start();
		$MySqlLinkIdentifier1 = mysql_connect( $ip1, $login1, $password1) or die(mysql_error());
		$var_bench .= " cnx:".m_bench_end($t1);

		$t1 = m_bench_start();
		mysql_select_db("nel", $MySqlLinkIdentifier1) or die(mysql_error());
		$var_bench .= " sel:".m_bench_end($t1);
	}

	$t1 = m_bench_start();
	$resultat = mysql_query($requete, $MySqlLinkIdentifier1) or die(mysql_error());
	$var_bench .= " query:".m_bench_end($t1);

	$t1 = m_bench_start();
	$erreur = mysql_error($MySqlLinkIdentifier1);
	$var_bench .= " err:".m_bench_end($t1);
	if ($erreur != "")
	{
		echo "mysql_error : $error<br/>";
		echo "in : $requete<br/>";
    	die("php stopped");
	}
	$var_bench .= " <b>total:".m_bench_end($total)."</b><br/>\n";
	return $resultat;
}

function exec_game_db_requete($requete)
{
	global $var_bench;
	global $MySqlLinkIdentifier2;
	global $ip2, $login2, $password2;

	$total = m_bench_start();

	$var_bench .= "'".$requete."'";
	
	if(!isset($MySqlLinkIdentifier2)) {
		$t1 = m_bench_start();
		$MySqlLinkIdentifier2 = mysql_connect($ip2, $login2, $password2) or die (mysql_error());
		$var_bench .= "'".$requete."' cnx:".m_bench_end($t1);

		$t1 = m_bench_start();
		mysql_select_db ("nel", $MySqlLinkIdentifier2) or die(mysql_error());
		$var_bench .= " sel:".m_bench_end($t1);
	}
			
	$t1 = m_bench_start();
	$result = mysql_query($requete, $MySqlLinkIdentifier2);
	$var_bench .= " query:".m_bench_end($t1);
	
	$t1 = m_bench_start();
	$error = mysql_error($MySqlLinkIdentifier2);
	$var_bench .= " err:".m_bench_end($t1);
	if ($error != "")
	{
		echo "mysql_error : $error<br/>";
		echo "in : $requete<br/>";
    	die("php stopped");
	}
	$var_bench .= " <b>total:".m_bench_end($total)."</b><br/>\n";
	return $result;
}

?>
