<?php

if (!function_exists('fprintf')) {
   if (function_exists('vsprintf')) { // >= 4.1.0
       function fprintf() {
           $args = func_get_args();
           $fp = array_shift($args);
           $format = array_shift($args);
           return fwrite($fp, vsprintf($format, $args));
       }
   } else { // < 4.1.0
       function fprintf() {
           $args = func_get_args();
           $fp = array_shift($args);
           $format = array_shift($args);
           $code = '';
           for ($i = 0; $i < count($args); ++$i) {
               if ($code) {
                   $code .= ',';
               }
               $code .= '$args[' . $i . ']';
           }
           $code = 'return sprintf($format, ' . $code . ');';
           $rv = eval($code);
           return $rv ? fwrite($fp, $rv) : false;
       }
   }
}

function displayCacheFile($filename)
{
	$html_fp = fopen($filename, "rt");
	while (!feof($html_fp)):
		$line = fgets($html_fp, 2048);
		echo $line;
		$html_fp++;
	endwhile;
	fclose($html_fp);
}

function isCacheFileUpToDateDuration($filename,$duration)
{
// ace no cache
//return false;
	global $cache_dir;
	if(!file_exists($cache_dir))
		mkdir($cache_dir);
	//chmod($cache_dir, 0777)
	$fileModificationTime = -1;
	if(file_exists($filename) && filesize($filename)>0)
	{
		$fileModificationTime = time() - filemtime($filename);
		//$fileModificationTime -= 1800; //web date and fiel date are not sync ?!?
		/*
		printf("date : %d<br>",strtotime(date("Y-m-d H:i:s")));
		printf("filemtime : %d<br>",filemtime($filename));
		printf("fileModificationTime : %d<br>",$fileModificationTime);
		*/
		if($fileModificationTime<$duration)
			return true;
		else
			unlink($filename);
	}
	return false;
}

function isCacheFileUpToDate($filename)
{
// ace no cache
//return false;
	global $defaultCacheFileDuration;
	//return false;
	$res = isCacheFileUpToDateDuration($filename,$defaultCacheFileDuration); //update every 30 minutes
	return $res;
}

function validPage($page)
{
	if (!(eregi("^[a-z\/_\-]+$", $page))) return false;
  	if(strlen($page)>40) return false;
    return file_exists($page.".php");
}

function bench_start()
{
	global $bench_time_start;
	$bench_time_start = microtime_float();
}

function bench_end()
{
	global $bench_time_start;
	$bench_time_end = microtime_float();
	$bench_time = $bench_time_end - $bench_time_start;
	return sprintf("%.2f",$bench_time);
}

function microtime_float()
{
	list($usec, $sec) = explode(" ", microtime());
	return ((float)$usec + (float)$sec);
}

?>
