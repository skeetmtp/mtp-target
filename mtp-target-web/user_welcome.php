<?php
	include_once("helpers.php");
	
	$cacheFileName = $cache_dir."/user_welcome_".$user_id.".html";
	if(isCacheFileUpToDate($cacheFileName))
	{
		include($cacheFileName);
		return;
	}
	$html_fp = fopen($cacheFileName, "wt");

	boxBeginFp($html_fp,false);
	fprintf($html_fp,"Welcome <b>%s</b>... (<a href=\"user_logout.php\">logout</a>)<br>",$user_login);
	fprintf($html_fp,"Today score : %d<br>",userTodayScore($user_id));
	fprintf($html_fp,"Total score : %d<br>",userCurrentScore($user_id));
	fprintf($html_fp,"<b>");
	fprintf($html_fp,"<a href=\"?page=user_settings.php\">your settings</a><br>");
	fprintf($html_fp,"<a href=\"?page=stat_user.php&p_user_id=%d\">your stats</a><br>",$user_id);
	fprintf($html_fp,"</b>");
	boxEndFp($html_fp);

	fclose($html_fp);	  
	include($cacheFileName);	
?>
