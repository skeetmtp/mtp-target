<?php
include_once("compatibility-func.php");
include_once("menu.php");
include_once("lang.php");
include_once("todo-func.php");
include_once("login.php");


  $requete = "SELECT * FROM compatibility WHERE 1 and id like ".$id." ORDER BY date;";
  $resultat = exec_requete($requete);
  $ligne = mysql_fetch_array($resultat);
?>
Report info :<br>
<br>
problem : <?php echo $ligne[2]; ?><br>
context : <?php echo $ligne[3]; ?><br>
workaround : <?php echo $ligne[5]; ?><br>

<table width="75%" border="0">
	<tr>
		<td bgcolor="#dddddd"><b>&nbsp;Type&nbsp;</b></td>
		<td bgcolor="#dddddd"><b>&nbsp;Title&nbsp;</b></td>
		<td bgcolor="#dddddd"><b>&nbsp;Todo&nbsp;</b></td>
		<td bgcolor="#dddddd"><b>&nbsp;User&nbsp;</b></td>
	</tr>
<form method="post" action="todo-add-from-compatibility.php" name="insertForm">
	<tr>
      <input type="hidden" name="status" value="waiting" />
      <td bgcolor="#eeeeee">
        <select name="type" dir="ltr">
          <option value="client">client</option>
          <option value="server">server</option>
          <option value="www">www</option>
        </select>
      </td>
      <td bgcolor="#eeeeee"><textarea name="title" rows="7" cols="30" wrap="virtual" tabindex="1"></textarea></td>
      <td bgcolor="#eeeeee"><textarea name="todo" rows="7" cols="50" wrap="virtual" tabindex="1">
probleme[<?php echo $ligne[2]; ?>];
context[<?php echo $ligne[3]; ?>];
workaround[<?php echo $ligne[5]; ?>]
</textarea></td>
      <td bgcolor="#eeeeee">
        <select name="user" dir="ltr">
          <option value="Ace">Ace</option>
          <option value="Muf">Muf</option>
          <option value="Skeet">Skeet</option>
        </select>
      </td>
      <input type="hidden" name="id" value="<?php echo $id; ?>" />
	</tr>
</table>
<input type="submit" value="Move to TODO" tabindex="14" />
</form>





