<?php
  include_once("conf.inc.php");
  include_once("config.php");
  include_once("common.php");
  include_once("mysql-func.php");
  include_once("ingame_stats.php");
  include_once("cache_2_html.php");
  
  if(isset($page) && !validPage($page)) unset($page);
  if(isset($login) && !validInput($login)) unset($login);
  if(isset($password) && !validInput($password)) unset($password);
  if(isset($lang) && !validInput($lang)) unset($lang);

  if(isset($lang))
  {
      setcookie("mtp_target_lang",$lang,time()+3600*24*30);
  }
  include_once("lang.php");
  include_once("check_admin_login.php");
  if(isset($_COOKIE['mtp_target_default_page']))
  	$default_page = $HTTP_COOKIE_VARS['mtp_target_default_page'];
  if(isset($default_page))
  {
      $default_page = $default_later;
  }
  else
  {
      $default_page = $default_first_time;
      setcookie("mtp_target_default_page",$default_later,time()+3600*24*30);
  }
  if (!isset($page)) 
  	$page = $default_page;


  $menu_array = array (
    "news"  => array ("url"=>"?page=news-manager.php", "name"=>$menuLinkText_News),
    "home"  => array ("url"=>"?page=home.php", "name"=>$menuLinkText_Home),
    "download"  => array ("url"=>"http://mtptarget.free.fr/download.php", "name"=>$menuLinkText_Download),
    "sources"  => array ("url"=>"?page=source.php", "name"=>$menuLinkText_Sources),
    "screenshot"  => array ("url"=>"?page=screenshot.php", "name"=>$menuLinkText_Screenshot),
    "documents"  => array ("url"=>"?page=howto.php", "name"=>$menuLinkText_Documents),
    "forum"  => array ("url"=>"/forum", "name"=>$menuLinkText_Forum),
//    "compatibility"  => array ("url"=>"?page=compatibility-list.php", "name"=>$menuLinkText_Compatibility),
    "todo"  => array ("url"=>"?page=todo-manager.php", "name"=>$menuLinkText_Todo),
    "stats"  => array ("url"=>"?page=stats.php", "name"=>$menuLinkText_Stats),
    "contact"  => array ("url"=>"?page=contact.php", "name"=>$menuLinkText_Contact),
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
<table border="0"  cellpadding="0" cellspacing="2" width="100%">
<tr align="center">
    <td colspan="<?php echo count($menu_array);?>">
        <a href="index.php"><img src="<?php echo $image_dir; ?>/logo.png" ALT="Logo"></a>
        <a href="?page=<?php echo $page;?>&lang=en"><img src="<?php echo $image_dir; ?>/en.png" ALT="English"></a>
        <a href="?page=<?php echo $page;?>&lang=fr"><img src="<?php echo $image_dir; ?>/fr.png" ALT="French"></a>
    </td>
</tr>
<tr>

<table border="0"  cellpadding="0" cellspacing="5" width="100%">
<tr valign="top">
<td>

<table border="0"  cellpadding="0" cellspacing="0" width="100%">
<tr >
        <td align="center"><div id="menu">
        <?php
        
		if($html_fp = cache2Html($cache_dir."/ingame_stats_menu.html",$defaultScoresCacheFileDuration))
		{
			getStats($nbop, $nbrp, $nbs);
			fprintf($html_fp,$menuStat, $nbrp, $nbop, $nbs);
			flushCache2Html($html_fp);
		}
        	
        ?>
        </div></td>
</tr>
<tr>
        <td align="center"><div id="menu">
        <?php
                printf("| ");
                foreach($menu_array as $key => $value)
                {
                      printf("<a href=\"%s\">%s</a> | ",$value["url"],$value["name"]);
                }
        ?>
        
        </div></td>
</tr>
</table>

</td>
<td>
 		  <?php include("user_check.php"); ?>
</td>
</tr>
</table>

<tr>
<td colspan="<?php echo count($menu_array);?>" valign="top" align="left">
<div id="main">
