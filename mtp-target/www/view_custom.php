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
		$count = 0;
		while (false !== ($entry = $d->read())) 
		{
			if(($count%2)==0)
				$color = $table_row_even_color;
			else
				$color = $table_row_odd_color;
			
			$f = $dirname . $entry ;
			$t = filetype($f);
			if($t == "file")
			{
				if(strstr($entry,"ping_ball_")!="" && strstr($entry,".tga")!="" )
				{
					printf("<tr>");
					$entryjpg = str_replace(".tga", ".jpg", $entry);
					printf("<td bgcolor=\"%s\"><a href=\"../user_texture/$entry\"><img src=\"../user_texture/$entryjpg\" ALT=\"file\"></a>&nbsp;$entry&nbsp;</td>",$table_news_row_color);
					printf("</tr>");
					printf("\n");
					$count++;
				}
			}
		}
		$d->close();
		if($count==0)
		{
			printf("<tr>");
			printf("<td bgcolor=\"%s\">No file </td>",$table_news_row_color);
			printf("</tr>");
		}
	?>
</table>

</body>
</html>
