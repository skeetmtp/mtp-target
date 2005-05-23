<?php
require_once("news-func.php");

if(!CUser::instance()->admin()) die("For admin only");

newsRemoveEntry($id);
require_once("news-manager.php");


?>



