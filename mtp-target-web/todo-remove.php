<?php
session_start();

require_once("todo-func.php");
require_once("user.php");

if(!CUser::instance()->admin()) die("For admin only");

todoRemoveEntry($_GET['id']);
header("Location: index.php?page=todo-manager");

?>
