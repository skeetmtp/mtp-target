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
    <td valign="top" width="400">
    	<?php include("stat_player_of_the_day.php");?>
    	<?php include(sprintf("./lang/%s/news-header.php",$lang));?>
    </td>
</tr>
</table>




