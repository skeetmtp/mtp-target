<?php

$cache2HtmlFilename[0] = "";

function cache2Html($cacheFileName,$cacheDurationInSecond)
{
	global $cache2HtmlFilename;
	if(isCacheFileUpToDate($cacheFileName,$cacheDurationInSecond))
	{
		include($cacheFileName);
		return 0;
	}
	
	$html_fp = fopen($cacheFileName, "wt");
	$cache2HtmlFilename[$html_fp]=$cacheFileName;

	return $html_fp;

}	
function flushCache2Html($html_fp)
{
	global $cache2HtmlFilename;
	fclose($html_fp);	  
	include($cache2HtmlFilename[$html_fp]);
}

?>
