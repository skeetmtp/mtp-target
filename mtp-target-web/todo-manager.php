<?php

require_once("todo-func.php");

if(isset($_GET['order'])) $order = $_GET['order']; else $order = "status";
if(isset($_GET['descCount'])) $descCount = $_GET['descCount']; else $descCount = 0;

if(isset($_GET['id'])) $id = $_GET['id'];

$descCount = $descCount % 2;

$thisPage = sprintf("?page=todo-manager");
?>

<script type="text/javascript" language="javascript">
<!--
function confirmLink(theLink)
{
    return confirm('Do you really want to delete the todo?');
}
//-->
</script>

<table class="todo">
	<tr>
    <?php
  		printf("<td bgcolor=\"#ffffff\"><b>&nbsp;Pri&nbsp;</b></td>\n");
  		printf("<td bgcolor=\"#ffffff\"><b>&nbsp;<a href=\"%s&order=%s&descCount=%s\">Date&nbsp;</b></td>\n",$thisPage,"opendate",$order=="opendate"?($descCount+1):$descCount);
  		printf("<td bgcolor=\"#ffffff\"><b>&nbsp;<a href=\"%s&order=%s&descCount=%s\">Status</a>&nbsp;</b></td>\n",$thisPage,"status",$order=="status"?($descCount+1):$descCount);
        printf("<td bgcolor=\"#ffffff\"><b>&nbsp;<a href=\"%s&order=%s&descCount=%s\">Type</a>&nbsp;</b></td>\n",$thisPage,"type",$order=="type"?($descCount+1):$descCount);
  		printf("<td bgcolor=\"#ffffff\"><b>&nbsp;<a href=\"%s&order=%s&descCount=%s\">Title</a>&nbsp;</b></td>\n",$thisPage,"title",$order=="title"?($descCount+1):$descCount);
  		printf("<td bgcolor=\"#ffffff\"><b>&nbsp;<a href=\"%s&order=%s&descCount=%s\">Added By</a>&nbsp;</b></td>\n",$thisPage,"openby",$order=="openby"?($descCount+1):$descCount);
  		printf("<td bgcolor=\"#ffffff\"><b>&nbsp;<a href=\"%s&order=%s&descCount=%s\">Managed by</a>&nbsp;</b></td>\n",$thisPage,"closeby",$order=="closeby"?($descCount+1):$descCount);
        if(CUser::instance()->admin())
  	      printf("<th bgcolor=\"#ffffff\" colspan=\"3\"\"><b>&nbsp;Action&nbsp;</b></th>\n");
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
                             $color ="#cccccc";
                           else
                             $color ="#dddddd";
                      break;
                      case "open":
                           if(($count%2) == 0)
                             $color ="#ffcccc";
                           else
                             $color ="#ffdddd";
                      break;
                      case "close":
                           if(($count%2) == 0)
                             $color ="#ccffcc";
                           else
                             $color ="#ddffdd";
                      break;
                      case "later":
                           if(($count%2) == 0)
                             $color ="#cccccc";
                           else
                             $color ="#dddddd";
                      break;
                      default :
                           $color ="#cccccc";
                      break;
                  }
                  if($kind=="release")
                           $color = "#ffffdd";
				  echo "<a name='id".$ligne[0]."'></a>";
                  printf("<td bgcolor=\"%s\">&nbsp;%d&nbsp;</td>",$color,$priority);
                  $date = substr($ligne[5],0,4+2+2+2);//"0000-00-00";
                  printf("<td bgcolor=\"%s\">&nbsp;%s&nbsp;</td>",$color,$date);
                  printf("<td bgcolor=\"%s\">&nbsp;%s&nbsp;</td>",$color,$status);
                  printf("<td bgcolor=\"%s\">&nbsp;%s&nbsp;</td>",$color,$ligne[7]);
                  printf("<td bgcolor=\"%s\">&nbsp;<a href=\"%s&id=%s&order=%s#id%s\">%s</a>&nbsp;</td>",$color,$thisPage,$ligne[0],$order,$ligne[0],$ligne[8]);
                  printf("<td bgcolor=\"%s\">&nbsp;%s&nbsp;</td>",$color,$ligne[3]);
                  printf("<td bgcolor=\"%s\">&nbsp;%s&nbsp;</td>",$color,$ligne[4]);
                  if(CUser::instance()->admin())
                  {
                     printf("<td bgcolor=\"%s\">&nbsp;<a href=\"todo-remove.php?id=%s\" onclick=\"return confirmLink(this)\">del</a>&nbsp;",$color,$ligne[0]);
                     printf("<td bgcolor=\"%s\">&nbsp;<a href=\"?page=todo-admin&id=%s\">admin</a>&nbsp;",$color,$ligne[0]);
                     if($ligne[1]=="open")
                       printf("<td bgcolor=\"%s\">&nbsp;<a href=\"todo-close.php?id=%s&closeby=%s\">close</a>&nbsp;",$color,$ligne[0],CUser::instance()->login());
                     else  if($ligne[1]=="waiting" || $ligne[1]=="later")
                       printf("<td bgcolor=\"%s\">&nbsp;<a href=\"todo-open.php?id=%s&openby=%s\">open</a>&nbsp;",$color,$ligne[0],CUser::instance()->login());
                     else
                       printf("<td bgcolor=\"%s\">&nbsp;",$color);
                  }
                  printf("</td>");
                  printf("</tr>");

                  if(isset($id) && $id==$ligne[0])
                  {
                    printf("<tr>");
                    printf("<td colspan=9 bgcolor=\"#ccccff\">&nbsp;%s&nbsp;</td>",$ligne[2]);
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

