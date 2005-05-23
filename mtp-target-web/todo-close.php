<?php
session_start();

require_once("todo-func.php");
require_once("user.php");

if(!CUser::instance()->admin()) die("For admin only");

  $requete = "SELECT * FROM todo WHERE 1 and id like ".$_GET['id']." ORDER BY opendate;";
  $resultat = exec_requete($requete);
  $ligne = mysql_fetch_array($resultat);

if($ligne[1]!="close")
  $closedate = date("Y-m-d H:i:s");
todoCloseEntry($_GET['id'],$_GET['closeby'],$closedate);
header("Location: index.php?page=todo-manager");

?>
