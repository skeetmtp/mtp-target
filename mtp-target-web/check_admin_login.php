<?php

  if(isset($login) && isset($password))
  {
  }
  else
  {
  	if(isset($_COOKIE['mtp_target_admin_login']))
    	$login = $_COOKIE['mtp_target_admin_login'];
    else
    	$login = "";	
  	if(isset($_COOKIE['mtp_target_admin_password']))
	    $password = $_COOKIE['mtp_target_admin_password'];
	  else
    	$password = "";	
  }
  
  $requete = "SELECT * FROM user WHERE login='".$login."' AND password='".$password."';";
  $resultat=exec_requete($requete);

  if($ligne = mysql_fetch_array($resultat))
  {
    $login = $ligne[0];
    setcookie("mtp_target_admin_login",$login,time()+3600*24*30);
    setcookie("mtp_target_admin_password",$password,time()+3600*24*30);
  }
  else
  {
    unset($login);
    unset($password);
  }
    
  $logged=isset($login) && $login!="";



?>

