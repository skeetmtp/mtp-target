<?php 
include_once("stat_function.php");



$user_logged = false;
$fromLoginPage = false;
if (isset ($user_login) || isset ($user_password)) 
{
	$fromLoginPage = true;
} 
else 
{
	if (isset ($_COOKIE['mtp_target_user_id']))
		$user_id = $_COOKIE['mtp_target_user_id'];
	else
		$user_id= "";
	if (isset ($_COOKIE['mtp_target_user_login']))
		$user_login = $_COOKIE['mtp_target_user_login'];
	else
		$user_login = "";
	if (isset ($_COOKIE['mtp_target_user_password']))
		$user_password = $_COOKIE['mtp_target_user_password'];
	else
		$user_password = "";
}

if ((!isset ($user_login) || !validInput($user_login)) || (!isset ($user_password) || !validInput($user_password))) 
{
	//echo "user : ".$user_login."<br>";
	//echo "pass : ".$user_password."<br>";
	unset ($user_login);
	unset ($user_password);
	if ($fromLoginPage)
		echo "login or password not valid";
} 
else 
{

	$requete = "SELECT * FROM user WHERE Login='".$user_login."';";
	$resultat = exec_game_db_requete($requete);

	if ($ligne = mysql_fetch_array($resultat)) 
	{
		//echo "user found in db<br>";
		$user_login = $ligne[1];
		$user_crypted = $ligne[2];
		$user_id = userName2Uid($user_login);
		setcookie("mtp_target_user_id", $user_id, time() + 3600 * 24 * 30);
		setcookie("mtp_target_user_login", $user_login, time() + 3600 * 24 * 30);
		setcookie("mtp_target_user_password", $user_password, time() + 3600 * 24 * 30);
		/*
		echo "user  : ".$user_login;
		echo "<br>";
		echo "pass  : ".$user_password;
		echo "<br>";
		echo "crypt : ".$user_crypted;
		*/
		$user_logged = crypt($user_password, $user_crypted) == $user_crypted;
	}
	else 
	{
		if ($fromLoginPage)
			echo "invalid login or password";
		unset ($user_login);
		unset ($user_password);
		unset ($user_crypted);
	}
}

if (!$user_logged)
	include ("user_form.php");
else
	include ("user_welcome.php");

?>
