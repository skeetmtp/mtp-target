<?php
     switch(CUser::instance()->language())
     {
         case "fr":
              require_once("./lang/fr/howto.php");
         break;
         default :
              require_once("./lang/en/howto.php");
         break;
     }
?>

