<?php
include_once("compatibility-func.php");
include_once("menu.php");


  $requete = "SELECT * FROM compatibility WHERE 1 and id like ".$id." ORDER BY date;";
  $resultat = exec_requete($requete);
  $ligne = mysql_fetch_array($resultat);
?>

<form method="post" action="compatibility-modify.php" name="modifyForm">
<table width="75%" border="0">
	<tr>
		<td bgcolor="#dddddd"><b>&nbsp;Context&nbsp;</b></td>
		<td bgcolor="#dddddd"><b>&nbsp;Problem&nbsp;</b></td>
		<td bgcolor="#dddddd"><b>&nbsp;Solution&nbsp;</b></td>
	</tr>
	<tr>
      <input type="hidden" name="id" value="<?php echo $ligne[0]; ?>" />
      <input type="hidden" name="date" value="<?php echo $ligne[7]; ?>" />
      <td bgcolor="#eeeeee"><textarea name="context" rows="7" cols="30" wrap="virtual" tabindex="1"><?php echo $ligne[3]; ?></textarea></td>
      <td bgcolor="#eeeeee"><textarea name="problem" rows="7" cols="30" wrap="virtual" tabindex="1"><?php echo $ligne[2]; ?></textarea></td>
      <td bgcolor="#eeeeee"><textarea name="workaround" rows="7" cols="40" wrap="virtual" tabindex="1"><?php echo $ligne[5]; ?></textarea></td>
	</tr>
</table>
<table width="75%" border="0">
	<tr>
		<td bgcolor="#dddddd"><b>&nbsp;Status&nbsp;</b></td>
		<td bgcolor="#dddddd"><b>&nbsp;Approved&nbsp;</b></td>
		<td bgcolor="#dddddd"><b>&nbsp;Type&nbsp;</b></td>
	</tr>
	<tr>
      <td bgcolor="#eeeeee">
        <select name="status" dir="ltr">
          <option value="unread" <?php if(strcmp($ligne[4],"unread")==0) echo "selected"; ?>>unread</option>
          <option value="ok" <?php if(strcmp($ligne[4],"ok")==0) echo "selected"; ?>>ok</option>
        </select>
      </td>
      <td bgcolor="#eeeeee">
        <select name="approved" dir="ltr">
          <option value="0" <?php if(strcmp($ligne[6],"0")==0) echo "selected"; ?>>0</option>
          <option value="1" <?php if(strcmp($ligne[6],"1")==0) echo "selected"; ?>>1</option>
        </select>
      </td>
      <td bgcolor="#eeeeee">
        <select name="type" dir="ltr">
          <option value="software" <?php if(strcmp($ligne[1],"software")==0) echo "selected"; ?>>software</option>
          <option value="hardware" <?php if(strcmp($ligne[1],"hardware")==0) echo "selected"; ?>>hardware</option>
        </select>
      </td>
	</tr>
</table>
<input type="submit" value="Modify" tabindex="14" />
</form>




