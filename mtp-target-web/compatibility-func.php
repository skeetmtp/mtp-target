<?php
include_once("config.php");
include_once("mysql-func.php");

function compatibilityAddEntry($type,$problem,$context,$status,$workaround,$approved)
{
  $requete = sprintf("INSERT INTO compatibility (type,problem,context,status,workaround,approved,date) VALUES('%s','%s','%s','%s','%s','%s','%s');",mysql_escape_string($type),mysql_escape_string($problem),mysql_escape_string($context),mysql_escape_string($status),mysql_escape_string($workaround),mysql_escape_string($approved),date("Y-m-d H:i:s"));
  $resultat=exec_requete($requete);
  return $resultat;
}

function compatibilityremoveEntry($id)
{
  $requete = "DELETE FROM compatibility WHERE id=".mysql_escape_string($id).";";
  $resultat=exec_requete($requete);
  return $resultat;
}

function compatibilitymodifyEntry($id,$type,$problem,$context,$status,$workaround,$approved)
{
  $requestDate = date("Y-m-d H:i:s");
  $requete = "UPDATE compatibility SET type='".mysql_escape_string($type)."' , problem='".mysql_escape_string($problem)."' , context='".mysql_escape_string($context)."' , status='".mysql_escape_string($status)."' , workaround='".mysql_escape_string($workaround)."' , approved=".mysql_escape_string($approved)." where id=".mysql_escape_string($id).";";
  $resultat=exec_requete($requete);
  return $resultat;
}


?>
