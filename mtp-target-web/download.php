<?php


include_once("lang.php");

echo "<h3>$menuLinkText_Download</h3>";
?>

<table border="0">
	<tr><td bgcolor="<?php echo $table_head_color;?>"><b>&nbsp;Full version&nbsp;</b></td></tr>
	<?php

		$dirname = "./download/";
		$d = dir($dirname);
		$count = 0;
		while (false !== ($entry = $d->read())) 
		{
			$count++;
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
					printf("<td bgcolor=\"%s\">&nbsp;<a href=\"$f\"><img src=\"download.png\" ALT=\"file\"> $entry</a>&nbsp;</td>",$color);
				}
			}
		}
		$d->close();
	?>
	<tr><td><b>&nbsp;&nbsp;</b></td></tr>
	<tr><td bgcolor="<?php echo $table_head_color;?>"><b>&nbsp;update only&nbsp;</b></td></tr>
	<?php

		$dirname = "./download/";
		$d = dir($dirname);
		$count = 0;
		while (false !== ($entry = $d->read())) 
		{
			$count++;
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
					printf("<td bgcolor=\"%s\">&nbsp;<a href=\"$f\"><img src=\"download.png\" ALT=\"file\"> $entry</a>&nbsp;</td>",$color);
				}
			}
		}
		$d->close();
	?>
	<tr><td><b>&nbsp;&nbsp;</b></td></tr>
</table>





