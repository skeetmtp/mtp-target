<?php
	include_once("stat_function.php");
	
	if(!$user_logged)
	{
		printf("<br>\n");
		printf("<br>\n");
		printf("<center><h2>Vous devez etre loggé pour ajouter une texture !<center></h2>\n");
		printf("<br>\n");
		printf("<br>\n");
		printf("<br>\n");
		printf("<br>\n");
		return;
	}

?>

<p>Cette page vous permet d'ajouter votre <a href="view_custom.php">propre texture</a> a votre pingoo!</p>

<p>SVP, lisez bien les regles suivants, sinon ca ne marchera pas :</p>

<p>- Telechargez la texture par defaut au format <a href="http://mtptarget.free.fr/screenshot/ping_ball_blue.tga">tga</a> ou <a href="http://mtptarget.free.fr/screenshot/ping_ball_blue.psd">psd</a> (photoshop avec calques)</p>

<p>- Modifiez votre texture avec votre outil de dessin favoris </p>

<p>- Sauvez la en <code>tga</code> . Le nom de fichier <b>*DOIT*</b> respecter la regle suivante : commencer par <code>ping_ball_</code> ensuite, le nom de votre texture, enfin <code>.tga</code>. par exemple, un nom de fichier valide est : <code>ping_ball_ryzom.tga</code></p>

<form enctype="multipart/form-data" action="http://mtp-target.dyndns.org/mtp-target/send.php" method="post">
 <input type="hidden" name="MAX_FILE_SIZE" value="300000" />
 <input type="hidden" name="user_login" value="<?php echo $user_login ?>" />
 - Ajouter la texture : <input name="userfile" type="file" />
 <input type="submit" value="Envoyer" />
</form>


