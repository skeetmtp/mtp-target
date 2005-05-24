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
	global $cache_dir;
	global $UseCache;
	if(!$UseCache) return false;
	
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
	global $UseCache;
	if(!$UseCache) return false;

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

// This function returns $text in the user language (french, english...)
// If the $text is not translated, it adds it in the php file and returns the default value ($text)
function lg($text)
{
	global $LanguageArray;

	if(CUser::instance()->language() == "") {
		die("call of lg('".$text."') to early, the user language is still unknown");
	}

	$path = "lang/language_array_".CUser::instance()->language().".php";

	if(!isset($LanguageArray)) {
		// try to load the language file
		if (file_exists($path)) {
			include_once($path);
		} else {
			// first time we use this language, create the file
			$LanguageArray = array();
		}
	}

	if(!isset($LanguageArray[$text])) {
		$LanguageArray[$text] = "**TBT_".$text."**";
		$fp = fopen($path, "w");
		fwrite($fp, "﻿<?php\r\n");
		fwrite($fp, '$LanguageArray = ');
		fwrite($fp, str_replace("\n", "\r\n", var_export($LanguageArray, true)));
		fwrite($fp, "\r\n?>\r\n");
		fclose($fp);
	}
	return $LanguageArray[$text];
}

?>
