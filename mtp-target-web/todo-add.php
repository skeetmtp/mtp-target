<?php
include_once("todo-func.php");

todoAddEntry($status,$todo,$user,$title,$type,$priority);
header("Location: index.php?page=todo-manager.php");


?>



