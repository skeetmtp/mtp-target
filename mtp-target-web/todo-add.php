<?php
include_once("todo-func.php");

todoAddEntry($status,$todo,$user,$title,$type);
header("Location: index.php?page=todo-manager.php");


?>



