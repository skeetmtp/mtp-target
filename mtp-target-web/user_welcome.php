<?php
	require_once("helpers.php");
	
	// only cache the scores
	$cacheFileName = $cache_dir."/user_welcome_".CUser::instance()->language()."_".CUser::instance()->uid().".html";
	if(!isCacheFileUpToDate($cacheFileName))
	{
		$html_fp = fopen($cacheFileName, "wt");
		fprintf($html_fp,$welcomeTodayScore,userTodayScore(CUser::instance()->uid()));
		fprintf($html_fp,$welcomeTotalScore,userCurrentScore(CUser::instance()->uid()));
		fclose($html_fp);
	}

	printf($welcomeUser,CUser::instance()->login());
	include($cacheFileName);
	printf($welcomeSettings);
	printf($welcomeStat,CUser::instance()->uid());

	// Special stuffs for admin only
	if(CUser::instance()->admin()) {
		printf('<tr><td class="r"><a href="?page=news-submit">Add a news</a></td></tr>');
		printf('<tr><td class="r"><a href="?page=todo-submit">Add a todo</a></td></tr>');
	}
	echo '<tr><td class="r"><a href="?logout=1">'.$lgLogout.'</a></td></tr>';
?>
