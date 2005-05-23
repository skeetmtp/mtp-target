<?php 

die("REMOVED BY ACE. OLD CODE ".__FILE__);
/*
require_once 'stat_function.php';

$user_logged = false;
$fromLoginPage = false;

echo "*******************".$_POST["user_login"]."*************";

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
	//echo "user : ".$user_login."<br/>";
	//echo "pass : ".$user_password."<br/>";
	unset ($user_login);
	unset ($user_password);
	if ($fromLoginPage)
		echo "login or password not valid";
}
else
{
	$user_logged = checkLoginPasswordCookie($user_login,$user_password);
	if ($fromLoginPage && !$user_logged)
		echo "invalid login or password";
	
}

if (!$user_logged)
	require_once ("user_form.php");
else
	require_once ("user_welcome.php");
*/
?>
