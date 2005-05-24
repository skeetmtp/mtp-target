<?php
/*
<h3>Official easy server statistics</h3>
<ul>
<li><a href="http://mtp-target.dyndns.org/mtp-target/stats.php?server=easy">Login</a></li>
<li><a href="http://mtp-target.dyndns.org/mtp-target/highscore.php?server=easy">Highscores</a></li>
</ul>
<h3>Official advanced server statistics</h3>
<ul>
<li><a href="http://mtp-target.dyndns.org/mtp-target/stats.php?server=adv">Login</a></li>
<li><a href="http://mtp-target.dyndns.org/mtp-target/highscore.php?server=adv">Highscores</a></li>
</ul>

<h3>Other statistics</h3>
<ul>
<li><a href="http://cia.navi.cx/stats/project/mtp-target">CVS</a></li>
<li><a href="http://stats.mtp-target.org/">Web site</a></li>
<li><a href="http://mtp-target.dyndns.org/mtp-target/view_custom.php">User textures</a></li>
</ul>

*/
echo '<h3>'.lg('GamesStat').'</h3><ul>';
echo '<li><a href="?page=stat_global">'.lg('GlobalStat').'</a></li>';
echo '<li><a href="?page=stat_ranking">'.lg('TotalScoreRanking').'</a></li>';
echo '<li><a href="?page=stat_ranking_of_the_day">'.lg('TodayScoreRanking').'</a></li>';
echo '<li><a href="?page=stat_ranking_per_score_of_the_day">'.lg('ScorePerDayRanking').'</a></li>';
echo '<li><a href="?page=stat_user_search">'.lg('SearchByLogin').'</a></li>';
echo '<li><a href="?page=stat_user_texture">'.lg('MostUsedTexture').'</a></li>';
echo '<li><a href="?page=logged">'.lg('OnlinePlayers').'</a></li>';
echo '<li><a href="?page=stat_server_up_list">'.lg('OnlineServers').'</a></li>';
echo '</ul>';

?>
