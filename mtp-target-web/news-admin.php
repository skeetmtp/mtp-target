<?php
require_once("news-func.php");

if(!CUser::instance()->admin()) die("For admin only");

  $requete = "SELECT * FROM news WHERE 1 and id like ".$id.";";
  $resultat = exec_requete($requete);
  $ligne = mysql_fetch_array($resultat);
?>

<form method="post" action="news-modify.php" name="modifyForm">
<table>
	<tr><td>User</td></tr>
	<tr>
      <input type="hidden" name="id" value="<?php echo $ligne[0]; ?>" />
      <input type="hidden" name="date" value="<?php echo $ligne[2]; ?>" />
      <td>
        <select name="user" dir="ltr">
          <option value="Ace" <?php if(strcmp($ligne[1],"Ace")==0) echo "selected"; ?>>Ace</option>
          <option value="Skeet" <?php if(strcmp($ligne[1],"Skeet")==0) echo "selected"; ?>>Skeet</option>
          <option value="Muf" <?php if(strcmp($ligne[1],"Muf")==0) echo "selected"; ?>>Muf</option>
        </select>
      </td>
    </tr>
	<tr><td>EN Title</td></tr>
    <tr><td><textarea name="en_title" rows="1" cols="120" wrap="virtual" tabindex="1"><?php echo $ligne[3]; ?></textarea></td></tr>
	<tr><td>EN Content</td></tr>
    <tr><td><textarea name="en_news" rows="7" cols="120" wrap="virtual" tabindex="1"><?php echo $ligne[4]; ?></textarea></td></tr>
	<tr><td>FR Titre</td></tr>
    <tr><td><textarea name="fr_title" rows="1" cols="120" wrap="virtual" tabindex="1"><?php echo $ligne[5]; ?></textarea></td></tr>
	<tr><td>FR Contenue By</b></td></tr>
    <tr><td><textarea name="fr_news" rows="7" cols="120" wrap="virtual" tabindex="1"><?php echo $ligne[6]; ?></textarea></td></tr>
</table>
<input type="submit" value="Modify" tabindex="14" />
</form>
