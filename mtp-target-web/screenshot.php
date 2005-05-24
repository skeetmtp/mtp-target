<?php
	echo "<center><br/>";
	echo lg('ScreenshotClick')."<br/>";
	echo lg('ScreenshotOver')."<br/>";
	for($i = 0; $i < 15; $i++) {
		$nb = sprintf("%02u", $i);
		$l = lg('Screenshot'.$nb);
		echo "<a href=\"$sshot_dir/screenshot_$nb.jpg\"><img src=\"$sshot_dir/screenshot_$nb.jpg\" width=\"200\" alt=\"$l\" title=\"$l\"></a>\n";
	}
	echo "</center>";
?>
