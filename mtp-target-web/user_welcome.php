<?php
	include_once("helpers.php");
	
	$cacheFileName = $cache_dir."/user_welcome_".$lang."_".$user_id.".html";
	if(isCacheFileUpToDate($cacheFileName))
	{
		include($cacheFileName);
		return;
	}
	$html_fp = fopen($cacheFileName, "wt");

	boxBeginFp($html_fp,false);
	fprintf($html_fp,$welcomeUser,$user_login);
	fprintf($html_fp,$welcomeTodayScore,userTodayScore($user_id));
	fprintf($html_fp,$welcomeTotalScore,userCurrentScore($user_id));
	fprintf($html_fp,"<b>");
	fprintf($html_fp,$welcomeSettings);
	fprintf($html_fp,$welcomeStat,$user_id);
	fprintf($html_fp,"</b>");
	boxEndFp($html_fp);

	fclose($html_fp);	  
	include($cacheFileName);	
?>
