<?php
include_once("lang.php");
include_once("todo-func.php");
include_once("login.php");
?>

<table width="75%" border="0">
	<tr>
		<td bgcolor="#dddddd"><b>&nbsp;Type&nbsp;</b></td>
		<td bgcolor="#dddddd"><b>&nbsp;Title&nbsp;</b></td>
		<td bgcolor="#dddddd"><b>&nbsp;Todo&nbsp;</b></td>
		<td bgcolor="#dddddd"><b>&nbsp;User&nbsp;</b></td>
	</tr>
<form method="post" action="todo-add.php" name="insertForm">
	<tr>
      <input type="hidden" name="status" value="waiting" />
      <td bgcolor="#eeeeee">
        <select name="type" dir="ltr">
          <option value="client">client</option>
          <option value="server">server</option>
          <option value="www">www</option>
          <option value="release">release</option>
        </select>
      </td>
      <td bgcolor="#eeeeee"><textarea name="title" rows="7" cols="30" wrap="virtual" tabindex="1"></textarea></td>
      <td bgcolor="#eeeeee"><textarea name="todo" rows="7" cols="50" wrap="virtual" tabindex="1"></textarea></td>
      <td bgcolor="#eeeeee">
        <select name="user" dir="ltr">
          <option value="Ace" <?php if(strcmp($login,"Ace")==0) echo "selected"; ?>>Ace</option>
          <option value="Muf" <?php if(strcmp($login,"Muf")==0) echo "selected"; ?>>Muf</option>
          <option value="Skeet" <?php if(strcmp($login,"Skeet")==0) echo "selected"; ?>>Skeet</option>
        </select>
      </td>
	</tr>
</table>
<input type="submit" value="Submit" tabindex="14" />
</form>

