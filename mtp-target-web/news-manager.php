<?php

$thisPage = sprintf("?page=news-manager");

$start=0;
if (isset($_GET['newsstart'])) $start = $_GET['newsstart'];
$end=$start+10;
$newer = max(0,$start-10);

echo '<table><tr><td>';

if(CUser::instance()->admin()) {
	echo "<script type=\"text/javascript\" language=\"javascript\">\n";
	echo "<!--\n";
	echo "function confirmLink(theLink) { return confirm('Do you really want to delete this news?'); }\n";
	echo "//-->\n";
	echo "</script>\n";
}

$resultat = exec_requete("SELECT * FROM news ORDER BY date DESC LIMIT ".$start.",".$end."");
while ($ligne = mysql_fetch_array($resultat))
{
	include("lang/".CUser::instance()->language()."/news-get.php");
	echo '<table class="news">';
	echo '<tr><td><div class="newshead1">'.$title.'</div>';
	echo '<div class="newshead2">'.$by.' '.$ligne[1].' the '.$ligne[2]; 
	if(CUser::instance()->admin())
		echo ' <a href="?page=news-admin&id='.$ligne[0].'">modify</a> <a href="?page=news-remove&id='.$ligne[0].'" onclick="return confirmLink(this)">delete</a>';
	echo '</div></td></tr>';
	echo '<tr><td class="newsbody"><p>'.str_replace("\n","<br />",$news).'</p></td></tr>';
	echo '</table>';
}

if ($start != 0) echo '<a href="?newsstart='.$newer.'">Newer news</a> ';
echo '<a href="?newsstart='.$end.'">Older news</a>';

echo '</td><td class="newsstaticmain">';
require_once("stat_player_of_the_day.php");
require_once("lang/".CUser::instance()->language()."/news-header.php");

?>



<!-- SiteSearch Google -->
<!-- disabled by ace
<form method="get" action="http://www.google.com/custom" target="_top">
<table border="0" CELLPADDING=2 CELLSPACING=2>
<tr><td nowrap="nowrap" valign="top" align="left" height="32">
<a href="http://www.google.com/">
<img src="http://www.google.com/logos/Logo_25wht.gif"
border="0" alt="Google"></img></a>
<br/>
<input type="hidden" name="domains" value="www.mtp-target.org"></input>
<input type="text" name="q" size="7" maxlength="255" value=""></input>
<input type="submit" name="sa" value="Search"></input>
</td></tr>
<tr>
<td nowrap="nowrap">
<table>
<tr>
<td>
<input type="radio" name="sitesearch" value="" checked="checked"></input>
<font size="-1" color="#000000">Web</font>
</td>
<td>
<input type="radio" name="sitesearch" value="www.mtp-target.org"></input>
<font size="-1" color="#000000">www.mtp-target.org</font>
</td>
</tr>
</table>
<input type="hidden" name="client" value="pub-6761473150430697"></input>
<input type="hidden" name="forid" value="1"></input>
<input type="hidden" name="ie" value="ISO-8859-1"></input>
<input type="hidden" name="oe" value="ISO-8859-1"></input>
<input type="hidden" name="safe" value="active"></input>
<input type="hidden" name="cof" value="GALT:#008000;GL:1;DIV:#336699;VLC:663399;AH:center;BGC:FFEEDD;LBGC:DDCCBB;ALC:0000FF;LC:0000FF;T:000000;GFNT:333399;GIMP:333399;LH:58;LW:100;L:http://mtptarget.free.fr/logo.png;S:http://www.mtp-target.org;FORID:1;"></input>
<input type="hidden" name="hl" value="en"></input>

</td></tr></table>
</form>
-->
<!-- SiteSearch Google -->


<!-- disabled by ace
<script type="text/javascript">
-->
<!--
google_ad_client = "pub-6761473150430697";
google_ad_width = 160;
google_ad_height = 600;
google_ad_format = "160x600_as";
google_ad_channel ="";
google_color_border = "700B09";
google_color_bg = "FFEEDD";
google_color_link = "CC0000";
google_color_url = "CC0000";
google_color_text = "000000";
//-->
<!-- disabled by ace
</script>
-->

<!-- disabled by ace
<script type="text/javascript"
  src="http://pagead2.googlesyndication.com/pagead/show_ads.js">
</script>
-->

</td></tr></table>


