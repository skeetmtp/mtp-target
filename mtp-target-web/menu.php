<?php
  include_once("conf.inc.php");
  include_once("config.php");
  include_once("mysql-func.php");

  if(!validPage($page)) unset($page);
  if(!validInput($login)) unset($login);
  if(!validInput($password)) unset($password);
  if(!validInput($lang)) unset($lang);

  if(isset($lang))
  {
      setcookie("mtp_target_lang",$lang,time()+3600*24*30);
  }
  include_once("lang.php");
  if(isset($login) && isset($password))
  {
  }
  else
  {
    $login = $HTTP_COOKIE_VARS["mtp_target_admin_login"];
    $password = $HTTP_COOKIE_VARS["mtp_target_admin_password"];
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
    
  $logged=isset($login) || $login!="";

  $default_page = $HTTP_COOKIE_VARS["mtp_target_default_page"];
  if(isset($default_page))
  {
      $default_page = $default_later;
  }
  else
  {
      $default_page = $default_first_time;
      setcookie("mtp_target_default_page",$default_later,time()+3600*24*30);
  }
  if (!isset($page)) $page = $default_page;


  $menu_array = array (
    "news"  => array ("url"=>"?page=news-manager.php", "name"=>$menuLinkText_News),
    "home"  => array ("url"=>"?page=home.php", "name"=>$menuLinkText_Home),
    "download"  => array ("url"=>"http://mtptarget.free.fr/download.php", "name"=>$menuLinkText_Download),
    "sources"  => array ("url"=>"http://cvs.gna.org/viewcvs/mtptarget/", "name"=>$menuLinkText_Sources),
    "screenshot"  => array ("url"=>"?page=screenshot.php", "name"=>$menuLinkText_Screenshot),
    "documents"  => array ("url"=>"?page=howto.php", "name"=>$menuLinkText_Documents),
    "forum"  => array ("url"=>"/forum", "name"=>$menuLinkText_Forum),
    "compatibility"  => array ("url"=>"?page=compatibility-list.php", "name"=>$menuLinkText_Compatibility),
    "todo"  => array ("url"=>"?page=todo-manager.php", "name"=>$menuLinkText_Todo),
    "stats"  => array ("url"=>"?page=stats.php", "name"=>$menuLinkText_Stats),
    "contact"  => array ("url"=>"?page=contact.php", "name"=>$menuLinkText_Contact),
    "admin"  => array ("url"=>"?page=admin.php", "name"=>$menuLinkText_Admin),
  );
?>
 
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN"
    "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html lang="<?php echo $lang ?>">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1">
<title>mtp-target</title>
<link rel="stylesheet" type="text/css" href="mtptarget.css">
</head>
<body>
<table border="0" cellpadding="5" cellspacing="0" width="100%">
<tr align="center">
    <td colspan="<?php echo count($menu_array);?>">
        <a href="index.php"><img src="logo.png" ALT="Logo"></a>
        <a href="?page=<?php echo $page;?>&lang=en"><img src="en.png" ALT="English"></a>
        <a href="?page=<?php echo $page;?>&lang=fr"><img src="fr.png" ALT="French"></a>
    </td>
</tr>
<tr>
        <td width="5%"><center><div id="menu">
        <?php
                printf("| ");
                foreach($menu_array as $key => $value)
                {
                    /*
                    if($key==$menu)
                      printf ("<td width=\"5%%\">%s</td>\n",$value["name"]);
                    else
                    */
                      printf("<a href=\"%s\">%s</a> | ",$value["url"],$value["name"]);
                }
        ?>
        </div></center></td>
</tr>
<tr>
<td colspan="<?php echo count($menu_array);?>" valign="top" align="left">
<div id="main">
<?php
     //printf("%s<br>",$lang);
?>
