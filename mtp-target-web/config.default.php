<?php
	function exec_requete($requete)
	{
	
		$MySqlLinkIdentifier = mysql_connect( "localhost", "base", "password" ) or die($die_message);
	
		mysql_select_db("base") or die($die_message);
	
		$resultat = mysql_query($requete) or die($die_message);
		$erreur = mysql_error();
		if ($erreur != "")
		{
	    		die($die_message);
		}
		return $resultat;
	}
?>
