<?php
	include_once("stat_function.php");
	
	if(!$user_logged)
	{
		printf("<br>\n");
		printf("<br>\n");
		printf("<center><h2>You must be logged to upload texture !<center></h2>\n");
		printf("<br>\n");
		printf("<br>\n");
		printf("<br>\n");
		printf("<br>\n");
		return;
	}

?>

<p>This page allow you to upload a <a href="view_custom.php">user texture</a> for your pingoo!</p>

<p>Please, read well the rules or it'll not work correctly:</p>

<p>- Get the default texture in <a href="http://mtptarget.free.fr/screenshot/ping_ball_blue.tga">tga</a> or <a href="http://mtptarget.free.fr/screenshot/ping_ball_blue.psd">psd</a> (photoshop with layers) format</p>

<p>- Modify the texture using your favorite drawing tool</p>

<p>- Save it in <code>tga</code> format. The file name <b>*must*</b> follow the name rules: it must starts with <code>ping_ball_</code> and, after, the name of your texture and <code>.tga</code>. For example, a valid name is <code>ping_ball_ryzom.tga</code></p>

<form enctype="multipart/form-data" action="http://mtp-target.dyndns.org/mtp-target/send.php" method="post">
 <input type="hidden" name="MAX_FILE_SIZE" value="300000" />
 <input type="hidden" name="user_login" value="<?php echo $user_login ?>" />
 - Select and upload the texture: <input name="userfile" type="file" />
 <input type="submit" value="Send File" />
</form>


