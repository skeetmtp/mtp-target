<?php
include_once("todo-func.php");
include_once("compatibility-func.php");

todoAddEntry($status,$todo,$user,$title,$type);
compatibilityRemoveEntry($id);
header("Location: index.php?page=todo-manager.php");


?>



