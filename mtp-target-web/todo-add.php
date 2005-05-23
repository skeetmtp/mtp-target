<?php
session_start();

require_once("todo-func.php");
require_once("user.php");

if(!CUser::instance()->admin()) die("For admin only");

todoAddEntry($_POST['status'],$_POST['todo'],$_POST['user'],$_POST['title'],$_POST['type'],$_POST['priority']);
header("Location: index.php?page=todo-manager");

?>
