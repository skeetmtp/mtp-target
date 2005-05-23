<?php
session_start();

require_once("todo-func.php");
require_once("user.php");

if(!CUser::instance()->admin()) die("For admin only");

  $requete = "SELECT * FROM todo WHERE id = ".$_GET['id']." ORDER BY opendate";
  $resultat = exec_requete($requete);
  $ligne = mysql_fetch_array($resultat);

todoOpenEntry($_GET['id'],$_GET['openby']);
header("Location: index.php?page=todo-manager");

?>
