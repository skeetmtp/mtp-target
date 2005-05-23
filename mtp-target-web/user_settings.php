<center>user settings...<br/></center>

<?php
require_once("stat_function.php");

if (!CUser::instance()->logged()) {
	die("Please login before");
}

$user_login=CUser::instance()->login();
$uid = CUser::instance()->uid();

	if(isset($user_texture_id))
	{
		if(validNumber($user_texture_id))
		{
			$requete = sprintf("UPDATE user SET Texture='%s' WHERE UId=%d;",Id2TextureName($user_texture_id),$uid);
			$resultat=exec_game_db_requete($requete);
		}
		else
			echo "Internal Error setting user_texture_id";
	}

	printf("<table border=\"0\" cellpadding=\"5\" cellspacing=\"1\" >\n");
	printf("<tr>\n");

	printf("<td >\n");
	$userTextureName = userTexture($uid);
	$userTextureUrl = $userTexturePath.$userTextureStartName.$userTextureName.".jpg";
	printf("<img src=\"%s\" ALT=\"user texture : %s\"><br/>\n",$userTextureUrl,$userTextureUrl);
	printf("</td>\n");

	printf("<td valign=\"bottom\">\n");
	printf($settingsChangeTexture);
	printf($settingsUploadTexture);
	//printf("<a href=\"?page=user_color_selection\">Change your color...</a><br/>");
	//printf("<a href=\"?page=user_delete\">Delete this account</a><br/>");
	printf($settingsMerge);
	printf($settingsRename);
	//printf("<a href=\"?page=test\">test</a><br/>");
	printf("</td>\n");

	printf("</tr>\n");
	printf("</table>\n");
	
	/*
	printf("TODO: set email<br/>\n");
	printf("TODO: [x] receive news by mail<br/>\n");
	*/
	
	
	
	
?>