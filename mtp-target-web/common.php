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
	
function DisplayCacheFile($filename)
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
	return (file_exists($filename) && filesize($filename)>0 && (strtotime(date("Y-m-d H:i:s")) - filemtime($filename))<$duration);
}

function isCacheFileUpToDate($filename)
{
	global $cache_dir;
	if(!file_exists($cache_dir))
		mkdir($cache_dir);
	//chmod($cache_dir, 0777)
	//return false;
	$res = isCacheFileUpToDateDuration($filename,5*60);
	return $res;
}


?>