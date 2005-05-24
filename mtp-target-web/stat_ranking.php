<?php
	require_once("stat_function.php");

	if(isset($_GET['limit_base']))
		$limit_base = $_GET['limit_base'];	

	$limit_range=20;
	if(!isset($limit_base) || !validNumber($limit_base)) 
		$limit_base=0;

	$cacheFileName = $cache_dir."/stat_ranking_".$limit_base.".html";
	if(isCacheFileUpToDate($cacheFileName))
	{
		require_once($cacheFileName);
		return;
	}
	$html_fp = fopen($cacheFileName, "wt");

	$thisPage = sprintf("?page=stat_ranking");
	
	//$result = exec_game_db_requete("select * from user ORDER BY Score LIMIT 0 , 30 where 1");
	fprintf($html_fp,"<table class=\"stat\">");
	$result = exec_game_db_requete(sprintf("select * from user ORDER BY Score DESC LIMIT %d , %d ",$limit_base,$limit_range));
	fprintf($html_fp,"<th>".lg('Rank')."</th>");
	fprintf($html_fp,"<th>".lg('Login')."</th>");
	fprintf($html_fp,"<th>".lg('Score')."</th>");
	$line_count = 1;
	while ($line = mysql_fetch_array($result))
	{
		fprintf($html_fp,"<tr>");
		fprintf($html_fp,"<td>%s</td>",$limit_base+$line_count);
		fprintf($html_fp,"<td><a href=\"?page=stat_user&p_user_id=%d\">%s</a></td>",$line[0],$line[1]);
		fprintf($html_fp,"<td>%s</td>",$line[6]);
		fprintf($html_fp,"</tr>");
		$line_count++;
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
