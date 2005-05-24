<?php

$menu_array = array (
	"MenuNews"  => "?page=news-manager",
	"MenuHome"  => "?page=home",
	"MenuDownload" => "http://mtptarget.free.fr/download.php",
	"MenuSources"  => "?page=source",
	"MenuScreenshots"  => "?page=screenshot",
	"MenuDocuments"  => "?page=howto",
	"MenuForum"  => $forumURL,
	"MenuTodo"  => "?page=todo-manager",
	"MenuStats"  => "?page=stats",
	"MenuContact"  => "?page=contact",
);

?>
<body>

<table class="head">
<tr>
<td class="logo">
<a href="index.php"><img src="<?php echo $image_dir; ?>/logo.png" alt="Mtp Target" /></a>
<?php

$url=$_SERVER['REQUEST_URI'];
if(false === strpos($url,'?')) {
	$url .= "?lang=";
} else {
	$pos = strpos($url,'lang=');
	if(false === $pos) {
		$url .= "&lang=";
	} else {
		$url=substr($url,0,$pos);
		$pos2 = strpos($_SERVER['REQUEST_URI'],'&',$pos);
		if(false !== $pos2) {
			$url .= substr($_SERVER['REQUEST_URI'],$pos2+1)."&";
		}
		$url .= "lang=";
	}
}

echo '<a href="'.$url.'en"><img src="'.$image_dir.'/en.png" alt="English" /></a>'."\n";
echo '<a href="'.$url.'fr"><img src="'.$image_dir.'/fr.png" alt="'.utf8_encode("Français").'" /></a>'."\n";
?>
</td>
</tr>
</table>

<?php
echo '<div class="divlogin"><table class="tablelogin">';
if (CUser::instance()->logged()) {
	require_once ("user_welcome.php");
} else {
	CUser::instance()->displayLoginForm();
}
echo '</table></div>';
?>


<table class="menu">
<tr>
<td>

<table class="menu">
<tr>
		<td>
        <?php
        $first = true;
        foreach($menu_array as $key => $value) {
        	if($first) $first = false;
        	else echo " | ";
        	echo '<a href="'.$value.'">'.lg($key).'</a>';
        }

       ?>

        </td>
</tr>
<tr>
        <td>
        <?php

        if($enableSql && $html_fp = cache2Html($cache_dir."/ingame_stats_menu_".CUser::instance()->language().".html",$defaultScoresCacheFileDuration))
        {
        	getStats($nbop, $nbrp, $nbs);
        	fprintf($html_fp, "<a href=\"?page=stat_ranking\">%d %s</a>", $nbrp, lg('RegisteredPlayers'));
        	fprintf($html_fp, " | <a href=\"?page=logged\">%d %s</a>", $nbop, lg('OnlinePlayers'));
        	fprintf($html_fp, " | <a href=\"?page=stat_global\">%d %s</a>", $nbs, lg('PLayedGames'));
        	flushCache2Html($html_fp);
        }
        ?>
        </td>
</tr>
</table>

</td>

</tr>
</table>
