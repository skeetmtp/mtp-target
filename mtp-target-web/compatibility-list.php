<?php
include_once("lang.php");
include_once("compatibility-func.php");


if($logged)
{
  printf("<a href=\"?page=compatibility-manager.php\">Admin all.</a><br>");
}

?>



<b><a href="?page=compatibility-submit.php">Send Feedback.</a></b>

<p><?php echo $compSummary_Hardware ?></p>

<table width="75%" border="0">
	<tr>
		<td bgcolor="<?php echo $table_head_color;?>"><b>&nbsp;Hardware&nbsp;</b></td>
		<td bgcolor="<?php echo $table_head_color;?>"><b>&nbsp;Problem&nbsp;</b></td>
		<td bgcolor="<?php echo $table_head_color;?>"><b>&nbsp;Work&nbsp;around&nbsp;/&nbsp;special&nbsp;settings&nbsp;</b></td>
	</tr>
      <?php
            $requete = "SELECT * FROM compatibility WHERE 1 AND type like 'hardware' AND approved like '1' ORDER BY date;";
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
                  printf("<td bgcolor=\"%s\">&nbsp;%s&nbsp;</td>",$color,$ligne[5]);
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


<p>Here are the list of software tested with Mtp Target</p>

<p></p>
<table width="75%" border="0">
	<tr>
		<td bgcolor="<?php echo $table_head_color;?>"><b>&nbsp;Software&nbsp;</b></td>
		<td bgcolor="<?php echo $table_head_color;?>"><b>&nbsp;Problem&nbsp;</b></td>
		<td bgcolor="<?php echo $table_head_color;?>"><b>&nbsp;Work around / special settings&nbsp;</b></td>
	</tr>
      <?php
            $requete = "SELECT * FROM compatibility WHERE 1 AND type like 'software' AND approved like '1' ORDER BY date;";
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
                  printf("<td bgcolor=\"%s\">&nbsp;%s&nbsp;</td>",$color,$ligne[5]);
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


