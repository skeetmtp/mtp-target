<?php

$menu_array = array (
"news"  => array ("url"=>"?page=news-manager", "name"=>$menuLinkText_News),
"home"  => array ("url"=>"?page=home", "name"=>$menuLinkText_Home),
"download"  => array ("url"=>"http://mtptarget.free.fr/download.php", "name"=>$menuLinkText_Download),
"sources"  => array ("url"=>"?page=source", "name"=>$menuLinkText_Sources),
"screenshot"  => array ("url"=>"?page=screenshot", "name"=>$menuLinkText_Screenshot),
"documents"  => array ("url"=>"?page=howto", "name"=>$menuLinkText_Documents),
"forum"  => array ("url"=>$forumURL, "name"=>$menuLinkText_Forum),
//    "compatibility"  => array ("url"=>"?page=compatibility-list.php", "name"=>$menuLinkText_Compatibility),
"todo"  => array ("url"=>"?page=todo-manager", "name"=>$menuLinkText_Todo),
"stats"  => array ("url"=>"?page=stats", "name"=>$menuLinkText_Stats),
"contact"  => array ("url"=>"?page=contact", "name"=>$menuLinkText_Contact),
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
                echo "| ";
                foreach($menu_array as $key => $value)
                {
                      echo '<a href="'.$value["url"].'">'.$value["name"].'</a> | ';
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
			fprintf($html_fp,$menuStat, $nbrp, $nbop, $nbs);
			flushCache2Html($html_fp);
		}
        ?>
        </td>
</tr>
</table>

</td>

</tr>
</table>
