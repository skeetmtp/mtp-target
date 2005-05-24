<?php

require_once('header.php');

require_once('menu.php');

// Uncomment this to have debug information
//echo '<hr/>'.CUser::instance()->dbg.'<hr/>';

$page = isset($_GET["page"]) ? $_GET["page"] : $default_page;

if(!validPage($page)) die("good try '".$page."'");

require_once($page.".php");

require_once('footer.php');
?>
