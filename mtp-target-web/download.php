<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN"
    "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html lang="en">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1">
<title>mtp-target</title>
<link rel="stylesheet" type="text/css" href="mtptarget.css">
</head>
<body>
<table border="0" cellspacing="0" width="100%">
<tr align="center">
    <td>
        <a href="http://www.mtp-target.org"><img src="logo.png" ALT="Logo"></a>
    </td>
</tr>


<table border="0">
<tr><td>
<?php
echo "<h3>Download</h3>";
?>
</tr></td>

	<tr><td><b>Windows Full version</b></td></tr>
	<?php
		function returnFileSize($fn,$precision=2) {
			$sizeInBytes = filesize($fn);
			return ($sizeInBytes < 1024)?"$sizeInBytes bytes":round(($sizeInBytes / pow(1024,floor(log($sizeInBytes,1024)))),$precision)." ".substr(" KMGT",log($sizeInBytes,1024),1)."b";
		}

		$dirname = "./download/";
		$d = dir($dirname);
		$count = 0;
		while (false !== ($entry = $d->read())) 
		{
			$f = $dirname . $entry ;
			$t = filetype($f);
			if($t == "file")
			{
				if(strstr($entry,"update")=="" && strstr($entry,"mtp-target")!="" && strstr($entry,".exe")!="" )
				{
					printf("<tr>");
					printf("<td><a href=\"$f\"><img src=\"download.png\" ALT=\"file\"> $entry</a> (".returnFileSize($f).")</td>");
					printf("</tr>");
					$count++;
				}
			}
		}
		$d->close();
		if($count==0)
		{
			printf("<tr>");
			printf("<td>No file available</td>");
			printf("</tr>");
		}
	?>
	<tr><td><b>&nbsp;GNU/Linux Full version&nbsp;</b></td></tr>
	<?php

		$dirname = "./download/";
		$d = dir($dirname);
		$count = 0;
		while (false !== ($entry = $d->read())) 
		{
			$f = $dirname . $entry ;
			$t = filetype($f);
			if($t == "file")
			{
				if(strstr($entry,"update")=="" && strstr($entry,"mtp-target")!="" && strstr($entry,".tar.bz2")!="" )
				{
					printf("<tr>");
					printf("<td><a href=\"$f\"><img src=\"download.png\" ALT=\"file\"> $entry</a> (".returnFileSize($f).")</td>");
					printf("</tr>");
					$count++;
				}
			}
		}
		$d->close();
		if($count==0)
		{
			printf("<tr>");
			printf("<td>No file available</td>");
			printf("</tr>");
		}
	?>
	<tr><td><b>&nbsp;&nbsp;</b></td></tr>
	<tr><td><b>&nbsp;Windows Update&nbsp;</b></td></tr>
	<?php

		$dirname = "./download/";
		$d = dir($dirname);
		$count = 0;
		while (false !== ($entry = $d->read())) 
		{
			$f = $dirname . $entry ;
			$t = filetype($f);
			if($t == "file")
			{
				if(strstr($entry,"update")!="")
				{
					printf("<tr>");
					printf("<td><a href=\"$f\"><img src=\"download.png\" ALT=\"file\"> $entry</a> (".returnFileSize($f).")</td>");
					printf("</tr>");
					$count++;
				}
			}
		}
		$d->close();
		if($count==0)
		{
			printf("<tr>");
			printf("<td>No file available</td>");
			printf("</tr>");
		}
	?>
	<tr><td><b>&nbsp;&nbsp;</b></td></tr>
	<tr><td><b>&nbsp;Utils&nbsp;</b></td></tr>
	<?php

		$dirname = "./utils/";
		$d = dir($dirname);
		$count = 0;
		while (false !== ($entry = $d->read())) 
		{
			$f = $dirname . $entry ;
			$t = filetype($f);
			if($t == "file")
			{
				//if(strstr($entry,"update")=="" && strstr($entry,"mtp-target")==""  && strstr($entry,"index")=="" )
				{
					printf("<tr>");
					printf("<td><a href=\"$f\"><img src=\"download.png\" ALT=\"file\"> $entry</a> (".returnFileSize($f)."</td>");
					printf("</tr>");
					$count++;
				}
			}
		}
		$d->close();
		if($count==0)
		{
			printf("<tr>");
			printf("<td>No file available</td>");
			printf("</tr>");
		}
	?>
	<tr><td><b>&nbsp;&nbsp;</b></td></tr>
	<tr><td><b>&nbsp;Videos&nbsp;</b></td></tr>
	<?php

		$dirname = "./download/";
		$d = dir($dirname);
		$count = 0;
		while (false !== ($entry = $d->read())) 
		{
			$f = $dirname . $entry ;
			$t = filetype($f);
			if($t == "file")
			{
				if(strstr($entry,".avi")!="")
				{
					printf("<tr>");
					printf("<td><a href=\"$f\"><img src=\"download.png\" ALT=\"file\"> $entry</a> (".returnFileSize($f).")</td>");
					printf("</tr>");
					$count++;
				}
			}
		}
		$d->close();
		if($count==0)
		{
			printf("<tr>");
			printf("<td>No file available</td>");
			printf("</tr>");
		}
	?>
	<tr><td><b>&nbsp;&nbsp;</b></td></tr>
</table>

</body>
</html>
