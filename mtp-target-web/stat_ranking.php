<?php
include_once("stat_function.php");

include_once("lang.php");

     $table_news_bgcolor_color = "#666666";
     $table_news_head_color = "#FFEEDD";
     $table_news_row_color = "#FFFAEA";


	$thisPage = sprintf("?page=stat_ranking.php");
//	$thisPage = sprintf("scores.php?toto=0");
	//echo ":$toto:$limit_base<br>";
	if(!isset($limit_base)) $limit_base=0;
	if(!isset($limit_range)) $limit_range=30;
	
	
	//$result = exec_game_db_requete("select * from user ORDER BY Score LIMIT 0 , 30 where 1");
	echo "<table border=\"0\" bgcolor=\"$table_news_bgcolor_color\">";
	$result = exec_game_db_requete(sprintf("select * from user ORDER BY Score DESC LIMIT %d , %d ",$limit_base,$limit_range));
	printf("<tr>");
	printf("<td bgcolor=\"$table_news_head_color\">&nbsp;Rank&nbsp;</td>");
	printf("<td bgcolor=\"$table_news_head_color\">&nbsp;Login&nbsp;</td>");
	printf("<td bgcolor=\"$table_news_head_color\">&nbsp;Score&nbsp;</td>");
	printf("</tr>");
	$line_count = 1;
	while ($line = mysql_fetch_array($result))
	{
		printf("<tr>");
		printf("<td bgcolor=\"$table_news_row_color\">&nbsp;%s&nbsp;</td>",$limit_base+$line_count);
		printf("<td bgcolor=\"$table_news_row_color\">&nbsp;<b><a href=\"?page=stat_user.php&p_user_id=%d\">%s</a></b>&nbsp;</td>",$line[0],$line[1]);
		printf("<td bgcolor=\"$table_news_row_color\">&nbsp;%s&nbsp;</td>",$line[6]);
		printf("</tr>");
		$line_count++;
	}	
	printf("</table>");
	printf("<table>");
	printf("<tr>");
	$previous_base = $limit_base-$limit_range;
	if($previous_base<0)
		$previous_base=0;
	printf("<tr>");
	printf("<td>");
	printf("<a href=\"%s&limit_base=%d\">&lt;&lt;prev</a>",$thisPage,$previous_base);
	printf("</td>");
	printf("<td>");
	printf("&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;");
	printf("&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;");
	printf("</td>");
	printf("<td valign=\"right\">");
	printf("<a href=\"%s&limit_base=%d\">next&gt;&gt;</a>",$thisPage,$limit_base+$limit_range);
	printf("</td>");
	printf("</tr>");
	printf("</table>");
	
	



	
?>