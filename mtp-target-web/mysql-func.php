<?php

function exec_requete($requete)
{

	$MySqlLinkIdentifier = mysql_connect( "sql.tf-data.net", "mtp_target","meR2ni" ) or die($die_message);

	mysql_select_db("mtp_target") or die($die_message);

	$resultat = mysql_query($requete) or die($die_message);
	$erreur = mysql_error();
	if ($erreur != "")
	{
    die($die_message);
	}
	return $resultat;
}

function validInput($input)
{
    return (ereg("^[A-Za-z0-9]+$",$input));
}

function validPage($page)
{
    return (ereg("^[/\.A-Za-z0-9-]+$",$page));
}

?>
