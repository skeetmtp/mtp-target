<?php
	require_once("stat_function.php");

	if(isset($_GET['limit_base']))
		$limit_base = $_GET['limit_base'];	

	$limit_range=10;
	if(!isset($limit_base) || !validNumber($limit_base)) 
		$limit_base=0;
		
	$cacheFileName = $cache_dir."/stat_ranking_per_score_of_the_day_".$limit_base.".html";
	if(isCacheFileUpToDateDuration($cacheFileName,60*60*24))//once a day
	{
		require_once($cacheFileName);
		return;
	}
	$html_fp = fopen($cacheFileName, "wt");

	$thisPage = sprintf("?page=stat_ranking_per_score_of_the_day");
	
	//$result = exec_game_db_requete("select * from user ORDER BY Score LIMIT 0 , 30 where 1");
	fprintf($html_fp,"<table class=\"stat\">");
	//$requete = "SELECT user.UId,user.Login,SUM(user_session.Score) as s,session.Date FROM session,user_session,user WHERE session.Id=user_session.SessionId AND user_session.UId=user.UId AND TO_DAYS(NOW())-TO_DAYS(session.Date)=0 GROUP BY user_session.UId,TO_DAYS(session.Date) ORDER BY s DESC LIMIT $limit_base,$limit_range;";
	$requete = "SELECT user.UId,user.Login,SUM(user_session.Score) as s,session.Date FROM session,user_session,user WHERE session.Id=user_session.SessionId AND user_session.UId=user.UId GROUP BY user_session.UId,TO_DAYS(session.Date) ORDER BY s DESC LIMIT $limit_base,$limit_range;";
	$result = exec_game_db_requete($requete);
	fprintf($html_fp,"<th>".lg('Rank')."</th>");
	fprintf($html_fp,"<th>".lg('Login')."</th>");
	fprintf($html_fp,"<th>".lg('ScoreOfTheDay')."</th>");
	fprintf($html_fp,"<th>".lg('Day')."</td>");
	$line_count = 1;
	while ($line = mysql_fetch_array($result))
	{
		if($line[2]>0)
		{
			fprintf($html_fp,"<tr>");
			fprintf($html_fp,"<td>%s</td>",$limit_base+$line_count);
			fprintf($html_fp,"<td><a href=\"?page=stat_user&p_user_id=%d\">%s</a></td>",$line[0],$line[1]);
			fprintf($html_fp,"<td>%s</td>",$line[2]);
			$date = strtotime($line[3]);
			fprintf($html_fp,"<td><a href=\"?page=stat_day&p_uid=%d&p_year_to_stat=%s&p_month_to_stat=%s&p_day_to_stat=%s\">%s</a></td>",$line[0],date("Y",$date),date("m",$date),date("d",$date),date("M d Y",$date));
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
	if($limit_base>0) fprintf($html_fp,"<a href=\"%s&limit_base=%d\">&lt;&lt;prev</a>",$thisPage,$previous_base);
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
	require_once($cacheFileName);	

?>
