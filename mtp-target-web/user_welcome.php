<?php
	include_once("helpers.php");
	boxBegin();
	printf("Welcome <b>%s</b>... (<a href=\"user_logout.php\">logout</a>)<br>",$user_login);
	printf("Today score : %d<br>",userTodayScore($user_id));
	printf("Total score : %d<br>",userCurrentScore($user_id));
	printf("<b>");
	printf("<a href=\"?page=user_settings.php\">your settings</a><br>");
	printf("<a href=\"?page=stat_user.php&p_user_id=%d\">your stats</a><br>",$user_id);
	printf("</b>");
	boxEnd();
?>
