<?php
include_once("stat_function.php");

include_once("lang.php");

     $table_news_bgcolor_color = "#666666";
     $table_news_head_color = "#FFEEDD";
     $table_news_row_color = "#FFFAEA";

	//if(!isset($limit_range)) 
		$limit_range=10;
	if(!isset($limit_base) || !validNumber($limit_base)) 
		$limit_base=0;
		
	$cacheFileName = $cache_dir."/stat_ranking_per_score_of_the_day_".$limit_base.".html";
	if(isCacheFileUpToDate($cacheFileName))
	{
		include($cacheFileName);
		return;
	}
	$html_fp = fopen($cacheFileName, "wt");



	$thisPage = sprintf("?page=stat_ranking_per_score_of_the_day.php");
	
	
	//$result = exec_game_db_requete("select * from user ORDER BY Score LIMIT 0 , 30 where 1");
	fprintf($html_fp,"<table border=\"0\" bgcolor=\"$table_news_bgcolor_color\">");
	//$requete = "SELECT user.UId,user.Login,SUM(user_session.Score) as s,session.Date FROM session,user_session,user WHERE session.Id=user_session.SessionId AND user_session.UId=user.UId AND TO_DAYS(NOW())-TO_DAYS(session.Date)=0 GROUP BY user_session.UId,TO_DAYS(session.Date) ORDER BY s DESC LIMIT $limit_base,$limit_range;";
	$requete = "SELECT user.UId,user.Login,SUM(user_session.Score) as s,session.Date FROM session,user_session,user WHERE session.Id=user_session.SessionId AND user_session.UId=user.UId GROUP BY user_session.UId,TO_DAYS(session.Date) ORDER BY s DESC LIMIT $limit_base,$limit_range;";
	$result = exec_game_db_requete($requete);
	printf("<tr>");
	fprintf($html_fp,"<td bgcolor=\"$table_news_head_color\">&nbsp;Rank&nbsp;</td>");
	fprintf($html_fp,"<td bgcolor=\"$table_news_head_color\">&nbsp;Login&nbsp;</td>");
	fprintf($html_fp,"<td bgcolor=\"$table_news_head_color\">&nbsp;Score of the day&nbsp;</td>");
	fprintf($html_fp,"<td bgcolor=\"$table_news_head_color\">&nbsp;Day&nbsp;</td>");
	fprintf($html_fp,"</tr>");
	$line_count = 1;
	while ($line = mysql_fetch_array($result))
	{
		if($line[2]>0)
		{
			fprintf($html_fp,"<tr>");
			fprintf($html_fp,"<td bgcolor=\"$table_news_row_color\">&nbsp;%s&nbsp;</td>",$limit_base+$line_count);
			fprintf($html_fp,"<td bgcolor=\"$table_news_row_color\">&nbsp;<b><a href=\"?page=stat_user.php&p_user_id=%d\">%s</a></b>&nbsp;</td>",$line[0],$line[1]);
			fprintf($html_fp,"<td bgcolor=\"$table_news_row_color\">&nbsp;%s&nbsp;</td>",$line[2]);
			$date = strtotime($line[3]);
			fprintf($html_fp,"<td bgcolor=\"$table_news_row_color\">&nbsp;<b><a href=\"?page=stat_day.php&p_uid=%d&p_year_to_stat=%s&p_month_to_stat=%s&p_day_to_stat=%s\">%s</a></b>&nbsp;</td>",$line[0],date("Y",$date),date("m",$date),date("d",$date),date("M d Y",$date));
			fprintf($html_fp,"</tr>");
			$line_count++;
		}
	}	
	fprintf($html_fp,"</table>");
	fprintf($html_fp,"<table>");
	fprintf($html_fp,"<tr>");
	$previous_base = $limit_base-$limit_range;
	if($previous_base<0)
		$previous_base=0;
	fprintf($html_fp,"<tr>");
	fprintf($html_fp,"<td>");
	fprintf($html_fp,"<a href=\"%s&limit_base=%d\">&lt;&lt;prev</a>",$thisPage,$previous_base);
	fprintf($html_fp,"</td>");
	fprintf($html_fp,"<td>");
	fprintf($html_fp,"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;");
	fprintf($html_fp,"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;");
	fprintf($html_fp,"</td>");
	fprintf($html_fp,"<td valign=\"right\">");
	fprintf($html_fp,"<a href=\"%s&limit_base=%d\">next&gt;&gt;</a>",$thisPage,$limit_base+$limit_range);
	fprintf($html_fp,"</td>");
	fprintf($html_fp,"</tr>");
	fprintf($html_fp,"</table>");
	
	

	fclose($html_fp);	  
	include($cacheFileName);	


	
?>