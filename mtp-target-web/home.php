<?php
     switch(CUser::instance()->language())
     {
         case "fr":
              require_once("./lang/fr/home.php");
         break;
         default :
              require_once("./lang/en/home.php");
         break;
     }
?>

