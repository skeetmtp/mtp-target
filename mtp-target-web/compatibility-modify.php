<?php
include("compatibility-func.php");

compatibilityModifyEntry($id,$type,$problem,$context,$status,$workaround,$approved);
header("Location: index.php?page=compatibility-manager.php");

?>





