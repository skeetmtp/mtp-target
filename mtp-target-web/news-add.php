<?php
session_start();
require_once("news-func.php");
require_once("user.php");

if(!CUser::instance()->admin())
	die("For admin only");

newsAddEntry($_POST['user'],$_POST['en_title'],$_POST['en_news'],$_POST['fr_title'],$_POST['fr_news']);
header("Location: index.php?page=news-manager");

?>
