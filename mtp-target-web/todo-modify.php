<?php
include_once("todo-func.php");

  $requete = "SELECT * FROM todo WHERE 1 and id like ".$id." ORDER BY opendate;";
  $resultat = exec_requete($requete);
  $ligne = mysql_fetch_array($resultat);

if($status!=$ligne[1] && $status=="close")
  $closedate = date("Y-m-d H:i:s");
todoModifyEntry($id,$status,$todo,$openby,$closeby,$opendate,$closedate,$title,$type,$priority);
header("Location: index.php?page=todo-manager.php");

?>





