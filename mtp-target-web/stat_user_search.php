<center>user texture selection</center>
<br>

<?php
	include_once("stat_function.php");
	$base = 0;
	$searchName = "";
	$textureDisplayedCount = 20;
	if(isset($p_base) && validNumber($p_base))
		$base = $p_base;
	if(isset($p_search_name) && validInput($p_search_name))
		$searchName = $p_search_name;
		
	$nextBase = $base + $textureDisplayedCount;
	$prevBase = $base - $textureDisplayedCount;
	if($prevBase<0)
		$prevBase=0;


	printf("Search a user : <br>");
	printf("<form method=\"post\" action=\"?page=stat_user_search.php\" name=\"userSearchForm\">");
	printf("<input type=\"text\" name=\"p_search_name\" size=\"16\"/>");
	printf("<input type=\"hidden\" name=\"p_base\" value=\"%s\"/>",$base);
	printf("<input type=\"submit\" value=\"search\" tabindex=\"14\" />");
	printf("</form>");

	if($searchName!="")
	{
		$requete = sprintf("SELECT * FROM user WHERE Login LIKE '%%%s%%' LIMIT $base,$textureDisplayedCount",$searchName);
		printf("<b>%s</b> search result : <br>\n",$searchName);
	}
	else
		$requete = sprintf("SELECT * FROM user LIMIT $base,$textureDisplayedCount");

	printf("<table width=\"*\" border=\"0\" cellpadding=\"0\" cellspacing=\"3\">\n");
	
	if($searchName=="")
	{
		printf("<tr>\n");
		printf("<td align=\"left\">\n");
		printf("<a href=\"?page=stat_user_search.php&p_base=%d\">&lt&lt prev %d</a>\n",$prevBase,$textureDisplayedCount);
		printf("</td>\n");
		printf("<td>\n");
		printf("&nbsp");
		printf("</td>\n");
		printf("<td align=\"right\">\n");
		printf("<a href=\"?page=stat_user_search.php&p_base=%d\">next %d &gt&gt</a>\n",$nextBase,$textureDisplayedCount);
		printf("</td>\n");
		printf("</tr>\n");
	}

	printf("<tr>\n");
	printf("<td colspan=\"3\">\n");
	
	$resultat=exec_game_db_requete($requete);
	while($ligne = mysql_fetch_array($resultat))
	{
		$userName = $ligne[1];
		$userId = $ligne[0];
		printf("<b><a href=\"?page=stat_user.php&p_user_id=%s\">%s</a></b><br>\n",$userId,$userName);
	}
	printf("</td>\n");
	printf("</tr>\n");

	if($searchName=="")
	{
		printf("<tr>\n");
		printf("<td align=\"left\">\n");
		printf("<a href=\"?page=stat_user_search.php&p_base=%d\">&lt&lt prev %d</a>\n",$prevBase,$textureDisplayedCount);
		printf("</td>\n");
		printf("<td>\n");
		printf("&nbsp");
		printf("</td>\n");
		printf("<td align=\"right\">\n");
		printf("<a href=\"?page=stat_user_search.php&p_base=%d\">next %d &gt&gt</a>\n",$nextBase,$textureDisplayedCount);
		printf("</td>\n");
		printf("</tr>\n");
	}

	printf("</table>\n");

?>
	
