<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN"
    "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html lang="en">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1">
<title>mtp-target</title>
<link rel="stylesheet" type="text/css" href="http://www.mtp-target.org/mtptarget.css">
</head>
<body>

<table border="0">
<tr><td>
</tr></td>

	<tr><td bgcolor="<?php echo $table_news_head_color;?>"><b>&nbsp;Custom Textures available : &nbsp;</b></td></tr>
	<?php

		$dirname = "../user_texture/";
		$d = dir($dirname);
		while (false !== ($entry = $d->read())) 
		{
			$f = $dirname . $entry ;
			$t = filetype($f);
			if($t == "file")
			{
				if(strstr($entry,"ping_ball_")!="" && substr($entry,-3)=="tga" )
				{
					$modified = stat("../user_texture/$entry");
					$moddate = date("D M j G:i:s T Y",$modified[9]);
					printf("<tr>");
					$entryjpg = str_replace(".tga", ".jpg", $entry);
					printf("<td><a href=\"../user_texture/$entry\"><img src=\"../user_texture/$entryjpg\" ALT=\"file\"></a></td>");
					echo "<td><table><tr><td>Name: $entry</td></tr>";
					echo "<tr><td>Uploaded: $moddate</td></tr></table>";
					printf("</td></tr>");
					printf("\n");
				}
			}
		}
		$d->close();
	?>
</table>

</body>
</html>
