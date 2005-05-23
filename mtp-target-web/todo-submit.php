<?php
require_once("todo-func.php");
if(!CUser::instance()->admin()) die("For admin only");
?>

<table width="75%" border="0">
	<tr>
		<td bgcolor="#dddddd"><b>&nbsp;Priority&nbsp;</b></td>
		<td bgcolor="#dddddd"><b>&nbsp;Type&nbsp;</b></td>
		<td bgcolor="#dddddd"><b>&nbsp;Title&nbsp;</b></td>
		<td bgcolor="#dddddd"><b>&nbsp;Todo&nbsp;</b></td>
		<td bgcolor="#dddddd"><b>&nbsp;User&nbsp;</b></td>
	</tr>
<form method="post" action="todo-add.php" name="insertForm">
	<tr>
      <input type="hidden" name="status" value="waiting" />
      <td bgcolor="#eeeeee">
        <select name="priority" dir="ltr">
        <?php 
          for($i=0;$i<6;$i++)
          {
          	printf("<option value=\"%d\">%d</option>",$i,$i);
   	       //<option value="1">5</option>
   	  }
   	?>
        </select>
      </td>
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
          <option value="Ace">Ace</option>
          <option value="Muf">Muf</option>
          <option value="Skeet">Skeet</option>
        </select>
      </td>
	</tr>
</table>
<input type="submit" value="Submit" tabindex="14" />
</form>

