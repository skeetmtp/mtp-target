<?php 
include_once ("stat_function.php");

	echo "<p>Thank you !";
	if (isset ($user_texture_id)) 
	{
		if (validNumber($user_texture_id)) 
		{
			$userTextureName = Id2TextureName($user_texture_id);
			$userTexturePath = "http://mtp-target.dyndns.org/user_texture/";
			$userTextureUrl = $userTexturePath."ping_ball_".$userTextureName.".jpg";
			echo "<p>The texture : '".$userTextureName."' was uploaded(will be available on servers within 15 min).<br>\n";
			printf("<a href=\"?page=user_settings.php&user_texture_id=%s\"><img src=\"%s\" ALT=\"user texture(id=%d) : %s\"></a><br>\n",$user_texture_id,$userTextureUrl,$user_texture_id,$userTextureName);
			printf("<a href=\"?page=user_settings.php&user_texture_id=%s\">Click here to set this texture as your default one.</a><br>\n",$user_texture_id,$userTextureUrl,$user_texture_id,$userTextureName);
		}
		else
			echo "Internal Error setting user_texture_id";
	}
?>