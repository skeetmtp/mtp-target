<?php
include_once("news-func.php");

newsAddEntry($user,$en_title,$en_news,$fr_title,$fr_news);
header("Location: index.php?page=news-manager.php");


?>



