<center>user texture selection</center>
<br>

<?php
	include_once("stat_function.php");
	$uid = userName2Uid($user_login);
	$base = 0;
	$searchName = "";
	$uploadByName = "";
	$textureDisplayedCount = 20;
	if(isset($p_base) && validNumber($p_base))
		$base = $p_base;
	if(isset($p_search_name) && validInput($p_search_name))
		$searchName = $p_search_name;
	if(isset($p_upload_by_name) && validInput($p_upload_by_name))
		$uploadByName = $p_upload_by_name;
		
	$nextBase = $base + $textureDisplayedCount;
	$prevBase = $base - $textureDisplayedCount;
	if($prevBase<0)
		$prevBase=0;

	printf("<a href=\"?page=user_texture_selection.php&p_upload_by_name=%s\">Display only texture you have uploaded</a>\n",$user_login);
	printf("<br>");
	printf("<br>");

	printf("Search a texture : <br>");
	printf("<form method=\"post\" action=\"?page=user_texture_selection.php\" name=\"textureSearchForm\">");
	printf("<input type=\"text\" name=\"p_search_name\" size=\"16\"/>");
	printf("<input type=\"hidden\" name=\"p_base\" value=\"%s\"/>",$base);
	printf("<input type=\"submit\" value=\"search\" tabindex=\"14\" />");
	printf("</form>");

	if($searchName!="")
	{
		$requete = sprintf("SELECT * FROM texture WHERE Name LIKE '%%%s%%'",$searchName);
		printf("%s search result : <br>\n",$searchName);
	}
	else if($uploadByName!="")
	{
		$requete = sprintf("SELECT * FROM texture WHERE UploadBy='%s'",$uploadByName);
		printf("%s search result : <br>\n",$uploadByName);
	}
	else
		$requete = sprintf("SELECT * FROM texture LIMIT $base,$textureDisplayedCount");

	printf("<table width=\"*\" border=\"0\" cellpadding=\"0\" cellspacing=\"3\">\n");
	
	if($searchName=="" && $uploadByName=="")
	{
		printf("<tr>\n");
		printf("<td align=\"left\">\n");
		printf("<a href=\"?page=user_texture_selection.php&p_base=%d\">&lt&lt prev %d</a>\n",$prevBase,$textureDisplayedCount);
		printf("</td>\n");
		printf("<td>\n");
		printf("&nbsp");
		printf("</td>\n");
		printf("<td align=\"right\">\n");
		printf("<a href=\"?page=user_texture_selection.php&p_base=%d\">next %d &gt&gt</a>\n",$nextBase,$textureDisplayedCount);
		printf("</td>\n");
		printf("</tr>\n");
	}

	printf("<tr>\n");
	printf("<td colspan=\"3\">\n");
//	printf("<br>");
	
	//include("http://mtp-target.dyndns.org/mtp-target/texture_selection.php");
	$userTexturePath = "http://mtp-target.dyndns.org/user_texture/";

	$resultat=exec_game_db_requete($requete);
	while($ligne = mysql_fetch_array($resultat))
	{
		$userTextureName = $ligne[1];
		$userTextureUrl = $userTexturePath."ping_ball_".$userTextureName.".jpg";
		printf("<a href=\"?page=user_settings.php&user_texture_id=%s\"><img src=\"%s\" ALT=\"%s\" title=\"%s\"></a>\n",$ligne[0],$userTextureUrl,$userTextureName,$userTextureName);
	}
	printf("</td>\n");
	printf("</tr>\n");

	if($searchName=="" && $uploadByName=="")
	{
		printf("<tr>\n");
		printf("<td align=\"left\">\n");
		printf("<a href=\"?page=user_texture_selection.php&p_base=%d\">&lt&lt prev %d</a>\n",$prevBase,$textureDisplayedCount);
		printf("</td>\n");
		printf("<td>\n");
		printf("&nbsp");
		printf("</td>\n");
		printf("<td align=\"right\">\n");
		printf("<a href=\"?page=user_texture_selection.php&p_base=%d\">next %d &gt&gt</a>\n",$nextBase,$textureDisplayedCount);
		printf("</td>\n");
		printf("</tr>\n");
}
	printf("</table>\n");

?>
	
