<?php


include_once("lang.php");
include_once("news-func.php");

if($logged)
  echo '<a href="?page=news-submit.php">Add  a news</a><br>';

$thisPage = sprintf("?page=news-manager.php");
?>



<script type="text/javascript" language="javascript">
<!--
var confirmMsg  = 'Confirm deletion';
function confirmLink(theLink)
{
    // Confirmation is not required in the configuration file
    if (confirmMsg == '') {
        return true;
    }

    var is_confirmed = confirm(confirmMsg);
    if (is_confirmed)
    {
//        theLink.href += '&is_js_confirmed=1';
    }
    else
    {
//        theLink.href = '';
    }

    return is_confirmed;
}
//-->
</script>

<?php
     //printf("<b><a href=\"?page=change-log-parser.php\">%s</a></b>",$news_ChangeLog);

     //include("news-header.php");
?>
<br>
<br>
<table width="100%" border="0">
<tr>
    <td width="800" valign="top">
<?php


$requete = sprintf("SELECT * FROM news ORDER BY date DESC LIMIT 0,10;");
$resultat = exec_requete($requete);
$count = 0;
while ($ligne = mysql_fetch_array($resultat))
{
      include(sprintf("./lang/%s/news-get.php",$lang));
      printf("<table  width=\"100%%\" border=\"0\" bgcolor=\"%s\">",$table_news_bgcolor_color);
      printf("<tr bgcolor=\"%s\"><td><b><FONT size=4>%s</FONT></b><br>",$table_news_head_color,$title);
      if($logged)
          printf("<FONT size=1 color=\"#666666\" class=size1>By %s the %s <a href=\"?page=news-admin.php&id=%s\">modify</a> <a href=\"?page=news-remove.php&id=%s\" onclick=\"return confirmLink(this)\">delete</a></FONT></td></tr>",$ligne[1],$ligne[2],$ligne[0],$ligne[0]);
      else
          printf("<FONT size=1 color=\"#666666\" class=size1>By %s the %s </FONT></td></tr>",$ligne[1],$ligne[2]);
      printf("<tr><td bgcolor=\"%s\">&nbsp;%s&nbsp;</td></tr>",$table_news_row_color,str_replace("\n","<br>",$news));
      printf("</table><br>");
}

?>
    </td>
    <td align="center" valign="top" width="400">
    	<?php include("stat_player_of_the_day.php");?>
    	<?php include(sprintf("./lang/%s/news-header.php",$lang));?>



<!-- SiteSearch Google -->
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
<!-- SiteSearch Google -->



<script type="text/javascript"><!--
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
//--></script>
<script type="text/javascript"
  src="http://pagead2.googlesyndication.com/pagead/show_ads.js">
</script>

    </td>
</tr>
</table>



