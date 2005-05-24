<?php
ob_start ("ob_gzhandler");

error_reporting(E_ALL);
session_start();

include_once("conf.inc.php");
include_once("config.php");
include_once("common.php");
include_once("mysql-func.php");
include_once("ingame_stats.php");
require_once("cache_2_html.php");

require_once('user.php');

bench_start();

// Load and init the user
CUser::instance();

//ace include_once("check_admin_login.php");

if(isset($_COOKIE['mtp_target_default_page']))
	$default_page = $_COOKIE['mtp_target_default_page'];

if(isset($default_page))
{
	$default_page = $default_later;
}
else
{
	$default_page = $default_first_time;
	setcookie("mtp_target_default_page",$default_later,time()+3600*24*30);
}

echo "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
  "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" lang="en" xml:lang="en">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />

<meta name="keywords" content="Mtp Target, game, mmorpg, internet, online, video" />
<meta name="description" content="Website of the game Mtp target, an online video game up to 16 players." />

<title>Mtp Target</title>
<link rel="stylesheet" type="text/css" href="mtptarget.css" />
</head>
