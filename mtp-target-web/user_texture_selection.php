user texture selection<br>

<?php
	include_once("stat_function.php");
	$uid = userName2Uid($user_login);
	
	//include("http://mtp-target.dyndns.org/mtp-target/texture_selection.php");
	$userTexturePath = "http://mtp-target.dyndns.org/user_texture/";

	$requete = sprintf("SELECT * FROM texture");
	$resultat=exec_game_db_requete($requete);
	while($ligne = mysql_fetch_array($resultat))
	{
		$userTextureName = $ligne[1];
		$userTextureUrl = $userTexturePath."ping_ball_".$userTextureName.".jpg";
		printf("<a href=\"?page=user_settings.php&user_texture_id=%s\"><img src=\"%s\" ALT=\"user texture(id=%d) : %s\"></a>\n",$ligne[0],$userTextureUrl,$ligne[0],$userTextureName);
	}

?>
	
