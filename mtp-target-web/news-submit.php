<?php
include_once("lang.php");
include_once("news-func.php");
include_once("login.php");

?>

<form method="post" action="news-add.php" name="insertForm"></tr>
<table border="0">
	<tr><td bgcolor="#dddddd"><b>&nbsp;User&nbsp;</b></td></tr>
	<tr>
      <td bgcolor="#eeeeee">
        <select name="user" dir="ltr">
          <option value="Ace" <?php if(strcmp($login,"Ace")==0) echo "selected"; ?>>Ace</option>
          <option value="Muf" <?php if(strcmp($login,"Muf")==0) echo "selected"; ?>>Muf</option>
          <option value="Skeet" <?php if(strcmp($login,"Skeet")==0) echo "selected"; ?>>Skeet</option>
        </select>
      </td>
  </tr>
  <tr><td bgcolor="#dddddd"><b>&nbsp;En Title&nbsp;</b></td></tr>
  <tr><td bgcolor="#eeeeee"><textarea name="en_title" rows="1" cols="120" wrap="virtual" tabindex="1"></textarea></td></tr>
  <tr><td bgcolor="#dddddd"><b>&nbsp;En Content&nbsp;</b></td></tr>
  <tr><td bgcolor="#eeeeee"><textarea name="en_news" rows="7" cols="120" wrap="virtual" tabindex="1"></textarea></td></tr>
  <tr><td bgcolor="#dddddd"><b>&nbsp;Titre Fr&nbsp;</b></td></tr>
  <tr><td bgcolor="#eeeeee"><textarea name="fr_title" rows="1" cols="120" wrap="virtual" tabindex="1"></textarea></td></tr>
  <tr><td bgcolor="#dddddd"><b>&nbsp;Contenu Fr&nbsp;</b></td></tr>
  <tr><td bgcolor="#eeeeee"><textarea name="fr_news" rows="7" cols="120" wrap="virtual" tabindex="1"></textarea></td></tr>
</table>
<input type="submit" value="Submit" tabindex="14" />
</form>

