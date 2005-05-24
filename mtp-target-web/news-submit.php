<?php
require_once("news-func.php");

if(!CUser::instance()->admin()) die("For admin only");

?>

<form method="post" action="news-add.php" name="insertForm"></tr>
<table>
	<tr><td>User</td></tr>
	<tr>
      <td>
        <select name="user" dir="ltr">
          <option value="Ace">Ace</option>
          <option value="Muf">Muf</option>
          <option value="Skeet">Skeet</option>
        </select>
      </td>
  </tr>
  <tr><td>EN Title</td></tr>
  <tr><td><textarea name="en_title" rows="1" cols="120" wrap="virtual" tabindex="1"></textarea></td></tr>
  <tr><td>EN Content</td></tr>
  <tr><td><textarea name="en_news" rows="7" cols="120" wrap="virtual" tabindex="1"></textarea></td></tr>
  <tr><td>FR Title</td></tr>
  <tr><td><textarea name="fr_title" rows="1" cols="120" wrap="virtual" tabindex="1"></textarea></td></tr>
  <tr><td>FR Content</td></tr>
  <tr><td><textarea name="fr_news" rows="7" cols="120" wrap="virtual" tabindex="1"></textarea></td></tr>
</table>
<input type="submit" value="Submit" tabindex="14" />
</form>
