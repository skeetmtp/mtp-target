<?php
include_once("lang.php");
include_once("compatibility-func.php");
?>


<p><?php echo $bugSummary_Beware ?>
<a href="?page=compatibility.php"> <?php echo $bugLinkText_CompatibilityList ?></a></p>

<table width="75%" border="0">
	<tr>
		<td bgcolor="#dddddd"><b>&nbsp;<?php echo $compHeadText_Context ?>&nbsp;</b></td>
		<td bgcolor="#dddddd"><b>&nbsp;<?php echo $compHeadText_Problem ?>&nbsp;</b></td>
		<td bgcolor="#dddddd"><b>&nbsp;<?php echo $compHeadText_Solution ?>&nbsp;</b></td>
	</tr>
<form method="post" action="compatibility-add.php" name="insertForm">
	<tr>
      <input type="hidden" name="type" value="hardware" />
      <td bgcolor="#eeeeee"><textarea name="context" rows="7" cols="30" wrap="virtual" tabindex="1"><?php echo $compInfo_Context ?></textarea></td>
      <td bgcolor="#eeeeee"><textarea name="problem" rows="7" cols="30" wrap="virtual" tabindex="1"><?php echo $compInfo_Problem ?></textarea></td>
      <td bgcolor="#eeeeee"><textarea name="workaround" rows="7" cols="40" wrap="virtual" tabindex="1"><?php echo $compInfo_Solution ?></textarea></td>
	</tr>
</table>
<input type="submit" value="<?php echo $compButtonText_SendProblem ?>" tabindex="14" />
</form>

