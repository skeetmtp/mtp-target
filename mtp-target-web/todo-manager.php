<?php


include_once("lang.php");
include_once("todo-func.php");

if(!isset($order)) $order="status";
if(!isset($descCount)) $descCount=0;
$descCount = $descCount % 2;

if($logged)
  echo '<a href="?page=todo-submit.php">Add a new TODO</a><br>';

$thisPage = sprintf("?page=todo-manager.php");
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


<table width="100%" border="0">
	<tr  align="center">
    <?php
  		printf("<td width=\"5%%\" bgcolor=\"%s\"><b>&nbsp;Priority&nbsp;</b></td>\n",$table_todo_head_color);
  		printf("<td width=\"5%%\" bgcolor=\"%s\"><b>&nbsp;<a href=\"%s&order=%s&descCount=%s\">Date&nbsp;</b></td>\n",$table_todo_head_color,$thisPage,"opendate",$order=="opendate"?($descCount+1):$descCount);
  		printf("<td width=\"5%%\" bgcolor=\"%s\"><b>&nbsp;<a href=\"%s&order=%s&descCount=%s\">Status</a>&nbsp;</b></td>\n",$table_todo_head_color,$thisPage,"status",$order=="status"?($descCount+1):$descCount);
        printf("<td width=\"5%%\" bgcolor=\"%s\"><b>&nbsp;<a href=\"%s&order=%s&descCount=%s\">Type</a>&nbsp;</b></td>\n",$table_todo_head_color,$thisPage,"type",$order=="type"?($descCount+1):$descCount);
  		printf("<td width=\"60%%\" bgcolor=\"%s\"><b>&nbsp;<a href=\"%s&order=%s&descCount=%s\">Title</a>&nbsp;</b></td>\n",$table_todo_head_color,$thisPage,"title",$order=="title"?($descCount+1):$descCount);
  		printf("<td width=\"10%%\" bgcolor=\"%s\"><b>&nbsp;<a href=\"%s&order=%s&descCount=%s\">Added By</a>&nbsp;</b></td>\n",$table_todo_head_color,$thisPage,"openby",$order=="openby"?($descCount+1):$descCount);
  		printf("<td width=\"10%%\" bgcolor=\"%s\"><b>&nbsp;<a href=\"%s&order=%s&descCount=%s\">Managed by</a>&nbsp;</b></td>\n",$table_todo_head_color,$thisPage,"closeby",$order=="closeby"?($descCount+1):$descCount);
        if($logged)
  	      printf("<th width=\"10%%\" bgcolor=\"%s\" colspan=\"3\"\"><b>&nbsp;Action&nbsp;</b></th>\n",$table_todo_head_color);
    ?>
	</tr>
      <?php
      //approved like '1' and
            if( $descCount==0 )
                $desc = " DESC";
            else
                $desc = "";
            if($order=="opendate")
            	$requete = sprintf("SELECT * FROM todo ORDER BY %s%s ,closedate DESC, priority DESC;",$order,$desc);
            else
            	$requete = sprintf("SELECT * FROM todo ORDER BY %s%s ,closedate DESC, priority DESC, opendate DESC;",$order,$desc);
            $resultat = exec_requete($requete);
            $count = 0;
            while ($ligne = mysql_fetch_array($resultat))
            {
            	  $priority = $ligne[9];
                  $status = $ligne[1];
                  $kind = $ligne[7];
                  printf("<tr>");
                  switch($status)
                  {
                      case "waiting":
                           if(($count%2) == 0)
                             $color =$table_todo_wait_even_color;
                           else
                             $color =$table_todo_wait_odd_color;
                      break;
                      case "open":
                           if(($count%2) == 0)
                             $color =$table_todo_open_even_color;
                           else
                             $color =$table_todo_open_odd_color;
                      break;
                      case "close":
                           if(($count%2) == 0)
                             $color =$table_todo_close_even_color;
                           else
                             $color =$table_todo_close_odd_color;
                      break;
                      case "later":
                           if(($count%2) == 0)
                             $color =$table_todo_wait_even_color;
                           else
                             $color =$table_todo_wait_odd_color;
                      break;
                      default :
                           $color =$table_todo_wait_even_color;
                      break;
                  }
                  if($kind=="release")
                           $color =$table_todo_release_color;
				  echo "<a name='id".$ligne[0]."'></a>";
                  printf("<td bgcolor=\"%s\">&nbsp;%d&nbsp;</td>",$color,$priority);
                  $date = substr($ligne[5],0,4+2+2+2);//"0000-00-00";
                  printf("<td bgcolor=\"%s\">&nbsp;%s&nbsp;</td>",$color,$date);
                  printf("<td bgcolor=\"%s\">&nbsp;%s&nbsp;</td>",$color,$status);
                  printf("<td bgcolor=\"%s\">&nbsp;%s&nbsp;</td>",$color,$ligne[7]);
                  printf("<td bgcolor=\"%s\">&nbsp;<a href=\"%s&id=%s&order=%s#id%s\">%s</a>&nbsp;</td>",$color,$thisPage,$ligne[0],$order,$ligne[0],$ligne[8]);
                  printf("<td bgcolor=\"%s\">&nbsp;%s&nbsp;</td>",$color,$ligne[3]);
                  printf("<td bgcolor=\"%s\">&nbsp;%s&nbsp;</td>",$color,$ligne[4]);
                  if($logged)
                  {
                     printf("<td bgcolor=\"%s\">&nbsp;<a href=\"?page=todo-remove.php&id=%s\" onclick=\"return confirmLink(this)\">delete</a>&nbsp;",$color,$ligne[0]);
                     printf("<td bgcolor=\"%s\">&nbsp;<a href=\"?page=todo-admin.php&id=%s\">admin</a>&nbsp;",$color,$ligne[0]);
                     if($ligne[1]=="open")
                       printf("<td bgcolor=\"%s\">&nbsp;<a href=\"todo-close.php?id=%s&closeby=%s\">close</a>&nbsp;",$color,$ligne[0],$login);
                     else  if($ligne[1]=="waiting" || $ligne[1]=="later")
                       printf("<td bgcolor=\"%s\">&nbsp;<a href=\"todo-open.php?id=%s&openby=%s\">open</a>&nbsp;",$color,$ligne[0],$login);
                     else
                       printf("<td bgcolor=\"%s\">&nbsp;",$color);
                  }
                  printf("</td>");
                  printf("</tr>");

                  if(isset($id) && $id==$ligne[0])
                  {
                    printf("<tr>");
                    printf("<td colspan=9 bgcolor=\"%s\">&nbsp;%s&nbsp;</td>",$table_todo_select_color,$ligne[2]);
                    printf("</td>");
}
             $count++;
            }
      ?>
	<tr>
		<td><b>&nbsp;&nbsp;</b></td>
		<td><b>&nbsp;&nbsp;</b></td>
		<td><b>&nbsp;&nbsp;</b></td>
		<td><b>&nbsp;&nbsp;</b></td>
	</tr>
</table>

