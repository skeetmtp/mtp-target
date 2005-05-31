<?php
session_start();

require_once("todo-func.php");
require_once("user.php");

if(!CUser::instance()->admin()) die("For admin only");

  $requete = "SELECT * FROM todo WHERE 1 and id like ".$_POST['id']." ORDER BY opendate;";
  $resultat = exec_requete($requete);
  $ligne = mysql_fetch_array($resultat);

if($status!=$ligne[1] && $_POST['status']=="close")
  $closedate = date("Y-m-d H:i:s");
todoModifyEntry($_POST['id'],$_POST['status'],$_POST['todo'],$_POST['openby'],$_POST['closeby'],$_POST['opendate'],$_POST['closedate'],$_POST['title'],$_POST['type'],$_POST['priority']);
header("Location: index.php?page=todo-manager");

?>
