<?php
include_once("todo-func.php");
include_once("login.php");


  $requete = "SELECT * FROM todo WHERE 1 and id like ".$id." ORDER BY opendate;";
  $resultat = exec_requete($requete);
  $ligne = mysql_fetch_array($resultat);
?>

<form method="post" action="todo-modify.php" name="modifyForm">
<table width="75%" border="0">
	<tr>
		<td bgcolor="#dddddd"><b>&nbsp;Priority&nbsp;</b></td>
		<td bgcolor="#dddddd"><b>&nbsp;Status&nbsp;</b></td>
		<td bgcolor="#dddddd"><b>&nbsp;Type&nbsp;</b></td>
		<td bgcolor="#dddddd"><b>&nbsp;Title&nbsp;</b></td>
		<td bgcolor="#dddddd"><b>&nbsp;Todo&nbsp;</b></td>
		<td bgcolor="#dddddd"><b>&nbsp;Added By&nbsp;</b></td>
		<td bgcolor="#dddddd"><b>&nbsp;Managed By&nbsp;</b></td>
	</tr>
	<tr>
      <input type="hidden" name="id" value="<?php echo $ligne[0]; ?>" />
      <input type="hidden" name="opendate" value="<?php echo $ligne[5]; ?>" />
      <input type="hidden" name="closedate" value="<?php echo $ligne[6]; ?>" />
      <td bgcolor="#eeeeee">
        <select name="priority" dir="ltr">
        <?php 
          for($i=0;$i<6;$i++)
          {
          	printf("<option value=\"%d\" ",$i);
          	if($i==$ligne[9])
          	  printf(" selected");
          	printf(">%d</option>",$i);
          }
          //<option value="1" <?php if($ligne[9]==1) echo "selected"; ?>>1</option>
        ?>
        </select>
      </td>
      <td bgcolor="#eeeeee">
        <select name="status" dir="ltr">
          <option value="waiting" <?php if(strcmp($ligne[1],"waiting")==0) echo "selected"; ?>>waiting</option>
          <option value="open" <?php if(strcmp($ligne[1],"open")==0) echo "selected"; ?>>open</option>
          <option value="close" <?php if(strcmp($ligne[1],"close")==0) echo "selected"; ?>>close</option>
          <option value="later" <?php if(strcmp($ligne[1],"later")==0) echo "selected"; ?>>later</option>
        </select>
      </td>
      <td bgcolor="#eeeeee">
        <select name="type" dir="ltr">
          <option value="client" <?php if(strcmp($ligne[7],"client")==0) echo "selected"; ?>>client</option>
          <option value="server" <?php if(strcmp($ligne[7],"server")==0) echo "selected"; ?>>server</option>
          <option value="www" <?php if(strcmp($ligne[7],"www")==0) echo "selected"; ?>>www</option>
          <option value="release" <?php if(strcmp($ligne[7],"release")==0) echo "selected"; ?>>release</option>
          <option value="doc" <?php if(strcmp($ligne[7],"doc")==0) echo "selected"; ?>>doc</option>
        </select>
      </td>
      <td bgcolor="#eeeeee"><textarea name="title" rows="7" cols="30" wrap="virtual" tabindex="1"><?php echo $ligne[8]; ?></textarea></td>
      <td bgcolor="#eeeeee"><textarea name="todo" rows="7" cols="50" wrap="virtual" tabindex="1"><?php echo $ligne[2]; ?></textarea></td>
      <td bgcolor="#eeeeee">
        <select name="openby" dir="ltr">
          <option value="" <?php if(strcmp($ligne[3],"")==0) echo "selected"; ?>>None</option>
          <option value="Ace" <?php if(strcmp($ligne[3],"Ace")==0) echo "selected"; ?>>Ace</option>
          <option value="Muf" <?php if(strcmp($ligne[3],"Muf")==0) echo "selected"; ?>>Muf</option>
          <option value="Skeet" <?php if(strcmp($ligne[3],"Skeet")==0) echo "selected"; ?>>Skeet</option>
        </select>
      </td>
      <td bgcolor="#eeeeee">
        <select name="closeby" dir="ltr">
          <option value="" <?php if(strcmp($ligne[4],"")==0) echo "selected"; ?>>None</option>
          <option value="Ace" <?php if(strcmp($ligne[4],"Ace")==0) echo "selected"; ?>>Ace</option>
          <option value="Muf" <?php if(strcmp($ligne[4],"Muf")==0) echo "selected"; ?>>Muf</option>
          <option value="Skeet" <?php if(strcmp($ligne[4],"Skeet")==0) echo "selected"; ?>>Skeet</option>
        </select>
      </td>
	</tr>
</table>
<input type="submit" value="Modify" tabindex="14" />
</form>




