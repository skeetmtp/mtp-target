<?php
	require_once("helpers.php");
	
	// only cache the scores
	$cacheFileName = $cache_dir."/user_welcome_".CUser::instance()->language()."_".CUser::instance()->uid().".html";
	if(!isCacheFileUpToDate($cacheFileName))
	{
		$html_fp = fopen($cacheFileName, "wt");
		fprintf($html_fp,"<tr><td class=\"r\">%s : %d</td></tr>",lg('TodayScore'), userTodayScore(CUser::instance()->uid()));
		fprintf($html_fp,"<tr><td class=\"r\">%s : %d</td></tr>",lg('TotalScore'), userCurrentScore(CUser::instance()->uid()));
		fclose($html_fp);
	}

	printf("<tr><td class=\"r\">%s <b>%s</b>...</td></tr>",lg('Welcome'), CUser::instance()->login());
	include($cacheFileName);
	printf("<tr><td class=\"r\"><a href=\"?page=user_settings\">%s</a></td></tr>", lg('YourSettings'));
	printf("<tr><td class=\"r\"><a href=\"?page=stat_user&p_user_id=%d\">%s</a></td></tr>", CUser::instance()->uid(), lg('YourStat'));

	// Special stuffs for admin only
	if(CUser::instance()->admin()) {
		printf('<tr><td class="r"><a href="?page=news-submit">'.lg('AddNews').'</a></td></tr>');
		printf('<tr><td class="r"><a href="?page=todo-submit">'.lg('AddTodo').'</a></td></tr>');
	}
	echo '<tr><td class="r"><a href="?logout=1">'.lg('Logout').'</a></td></tr>';
?>
