<?php
include_once("lang.php");
include_once("compatibility-func.php");

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
	<tr>
		<td bgcolor="<?php echo $table_head_color;?>"><b>&nbsp;Context&nbsp;</b></td>
		<td bgcolor="<?php echo $table_head_color;?>"><b>&nbsp;Problem&nbsp;</b></td>
		<td bgcolor="<?php echo $table_head_color;?>"><b>&nbsp;Status&nbsp;</b></td>
		<td bgcolor="<?php echo $table_head_color;?>"><b>&nbsp;Workaround&nbsp;</b></td>
		<td bgcolor="<?php echo $table_head_color;?>"><b>&nbsp;Approved&nbsp;</b></td>
		<td bgcolor="<?php echo $table_head_color;?>" width=150><b>&nbsp;Date&nbsp;</b></td>
		<td bgcolor="<?php echo $table_head_color;?>"><b>&nbsp;Type&nbsp;</b></td>
		<th bgcolor="<?php echo $table_head_color;?>" align="center" colspan="3"><b>&nbsp;Action&nbsp;</b></th>
	</tr>
      <?php
      //approved like '1' and
            $requete = "SELECT * FROM compatibility ORDER BY date;";
            $resultat = exec_requete($requete);
            $count = 0;
            while ($ligne = mysql_fetch_array($resultat))
            {
                  if(($count%2)==0)
                    $color = $table_row_even_color;
                  else
                    $color = $table_row_odd_color;
                  printf("<tr>");
                  printf("<td bgcolor=\"%s\">&nbsp;%s&nbsp;</td>",$color,$ligne[3]);
                  printf("<td bgcolor=\"%s\">&nbsp;%s&nbsp;</td>",$color,$ligne[2]);
                  printf("<td bgcolor=\"%s\">&nbsp;%s&nbsp;</td>",$color,$ligne[4]);
                  printf("<td bgcolor=\"%s\">&nbsp;%s&nbsp;</td>",$color,$ligne[5]);
                  printf("<td bgcolor=\"%s\">&nbsp;<a href=\"compatibility-modify.php?id=%s&date=%s&context=%s&problem=%s&workaround=%s&status=%s&approved=%s&type=%s\">%s</a>&nbsp;</td>",$color,$ligne[0],$ligne[7],htmlspecialchars($ligne[3]),htmlspecialchars($ligne[2]),htmlspecialchars($ligne[5]),$ligne[4],$ligne[6]=="0"?"1":"0",$ligne[1],$ligne[6]);
                  printf("<td bgcolor=\"%s\">&nbsp;%s&nbsp;</td>",$color,$ligne[7]);
//                  printf("<td bgcolor=\"%s\">&nbsp;%s&nbsp;</td>",$color,$ligne[1]);
                  printf("<td bgcolor=\"%s\">&nbsp;<a href=\"compatibility-modify.php?id=%s&date=%s&context=%s&problem=%s&workaround=%s&status=%s&approved=%s&type=%s\">%s</a>&nbsp;</td>",$color,$ligne[0],$ligne[7],htmlspecialchars($ligne[3]),htmlspecialchars($ligne[2]),htmlspecialchars($ligne[5]),$ligne[4],$ligne[6],$ligne[1]=="hardware"?"software":"hardware",$ligne[1]);
                  printf("<td bgcolor=\"%s\">&nbsp;<a href=\"compatibility-remove.php?id=%s\" onclick=\"return confirmLink(this)\">delete</a>&nbsp;",$color,$ligne[0]);
                  printf("<td bgcolor=\"%s\">&nbsp;<a href=\"compatibility-admin.php?id=%s\">admin</a>&nbsp;",$color,$ligne[0]);
                  printf("<td bgcolor=\"%s\">&nbsp;<a href=\"compatibility-to-todo.php?id=%s\">to todo</a>&nbsp;",$color,$ligne[0]);
                  printf("</td>");
                  printf("</tr>");
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

