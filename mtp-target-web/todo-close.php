<?php
include_once("todo-func.php");

  $requete = "SELECT * FROM todo WHERE 1 and id like ".$id." ORDER BY opendate;";
  $resultat = exec_requete($requete);
  $ligne = mysql_fetch_array($resultat);

if($ligne[1]!="close")
  $closedate = date("Y-m-d H:i:s");
todoCloseEntry($id,$closeby,$closedate);
header("Location: index.php?page=todo-manager.php");

?>
