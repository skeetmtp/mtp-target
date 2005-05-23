<?php
require_once("news-func.php");

$id = $_POST['id'];

newsModifyEntry($id,$_POST['user'],$_POST['en_title'],$_POST['en_news'],$_POST['fr_title'],$_POST['fr_news']);
header("Location: index.php?page=news-manager");

?>
