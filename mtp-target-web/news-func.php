<?php
include_once("mysql-func.php");

function newsAddEntryEN($user,$title,$news)
{
  $requete = sprintf("INSERT INTO news (user,en_title,en_news,date) VALUES('%s','%s','%s','%s');",$user,$title,$news,date("Y-m-d H:i:s"));
  $resultat=exec_requete($requete);
  return $resultat;
}

function newsAddEntry($user,$en_title,$en_news,$fr_title,$fr_news)
{
  $requete = sprintf("INSERT INTO news (user,en_title,en_news,fr_title,fr_news,date) VALUES('%s','%s','%s','%s','%s','%s');",$user,$en_title,$en_news,$fr_title,$fr_news,date("Y-m-d H:i:s"));
  $resultat=exec_requete($requete);
  return $resultat;
}



function newsRemoveEntry($id)
{
  $requete = "DELETE FROM news WHERE id=".$id.";";
  $resultat=exec_requete($requete);
  return $resultat;
}

function newsModifyEntry($id,$user,$en_title,$en_news,$fr_title,$fr_news,$date)
{
  $requete = sprintf("UPDATE news SET user='%s' , en_title='%s' , en_news='%s' ,fr_title='%s' , fr_news='%s' , date='%s' where id=%s;",$user,$en_title,$en_news,$fr_title,$fr_news,$date,$id);
  $resultat=exec_requete($requete);
  return $resultat;
}


?>
