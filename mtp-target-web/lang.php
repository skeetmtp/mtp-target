<?php
if(!isset($lang))
{
	$lang = "en";
	if(!isset($HTTP_COOKIE_VARS["mtp_target_lang"]))
	{
	    if(strstr($_SERVER['HTTP_ACCEPT_LANGUAGE'],"en"))
		    $lang = "en";
	    if(strstr($_SERVER['HTTP_ACCEPT_LANGUAGE'],"fr"))
		    $lang = "fr";
	}
	else
		$lang = $HTTP_COOKIE_VARS["mtp_target_lang"];
}
include(sprintf("./lang/%s/lang.inc.php",$lang));

?>
