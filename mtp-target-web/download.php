<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN"
    "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html lang="<?php echo $lang ?>">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1">
<title>mtp-target</title>
<link rel="stylesheet" type="text/css" href="mtptarget.css">
</head>
<body>
<table border="0" cellpadding="5" cellspacing="0" width="100%">
<tr align="center">
    <td colspan="<?php echo count($menu_array);?>">
        <a href="http://www.mtp-target.org"><img src="logo.png" ALT="Logo"></a>
    </td>
</tr>
<tr>
<td colspan="<?php echo count($menu_array);?>" valign="top" align="left">
<div id="main">

<?php
  include_once("conf.inc.php");
  include_once("config.php");
  include_once("lang.php");

echo "<h3>$menuLinkText_Download</h3>";
?>

<table border="0">
	<tr><td bgcolor="<?php echo $table_news_head_color;?>"><b>&nbsp;Full version&nbsp;</b></td></tr>
	<?php

		$dirname = "./download/";
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
				if(strstr($entry,"update")=="")
				{
					printf("<tr>");
					printf("<td bgcolor=\"%s\">&nbsp;<a href=\"$f\"><img src=\"download.png\" ALT=\"file\"> $entry</a>&nbsp;</td>",$table_news_row_color);
					printf("</tr>");
					$count++;
				}
			}
		}
		$d->close();
		if($count==0)
		{
			printf("<tr>");
			printf("<td bgcolor=\"%s\">No file available</td>",$table_news_row_color);
			printf("</tr>");
		}
	?>
	<tr><td><b>&nbsp;&nbsp;</b></td></tr>
	<tr><td bgcolor="<?php echo $table_news_head_color;?>"><b>&nbsp;Update&nbsp;</b></td></tr>
	<?php

		$dirname = "./download/";
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
				if(strstr($entry,"update")!="")
				{
					printf("<tr>");
					printf("<td bgcolor=\"%s\">&nbsp;<a href=\"$f\"><img src=\"download.png\" ALT=\"file\"> $entry</a>&nbsp;</td>",$table_news_row_color);
					printf("</tr>");
					$count++;
				}
			}
		}
		$d->close();
		if($count==0)
		{
			printf("<tr>");
			printf("<td bgcolor=\"%s\">No file available</td>",$table_news_row_color);
			printf("</tr>");
		}
	?>
	<tr><td><b>&nbsp;&nbsp;</b></td></tr>
</table>




</div>
</td></tr>
</table>
</body>
</html>
