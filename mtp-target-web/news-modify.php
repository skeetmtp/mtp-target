<?php
include("news-func.php");

  $requete = "SELECT * FROM news WHERE 1 and id like ".$id.";";
  $resultat = exec_requete($requete);
  $ligne = mysql_fetch_array($resultat);

newsModifyEntry($id,$user,$en_title,$en_news,$fr_title,$fr_news,$date);
header("Location: index.php?page=news-manager.php");

?>





