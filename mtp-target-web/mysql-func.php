<?php

function auto_bench_start($id)
{
	global $auto_bench_time_start;
	$auto_bench_time_start[$id] = microtime_float();
}

function auto_bench_end($id)
{
	global $auto_bench_time_start;
	$bench_time_end = microtime_float();
	$bench_time = $bench_time_end - $auto_bench_time_start[$id];
	printf("bench test '%s' done in %.2f seconds<br>\n",$id,$bench_time);
}

function bench_start()
{
	global $bench_time_start;
	$bench_time_start = microtime_float();
}

function bench_end()
{
	global $bench_time_start;
	$bench_time_end = microtime_float();
	$bench_time = $bench_time_end - $bench_time_start;
	return sprintf("%.2f",$bench_time);
}


function microtime_float()
{
   list($usec, $sec) = explode(" ", microtime());
   return ((float)$usec + (float)$sec);
} 

function assertValidNumber($input)
{
	if(validNumber($input))
		return;
	else
    	die("input error : php stopped");
}

function validNumber($input)
{
  $res = (ereg("^[0-9]+$",$input));
  return $res;
}

function validInput($input)
{
  if(strlen($input)>20) return false;
  $res = (ereg("^[\-_\.A-Za-z0-9([)(|\]|])]+$",$input));
  //$res = (ereg("^[_\.A-Za-z0-9\-]+$",$input));
  /*
  if(!$res)
  	echo "not a valid input : ".$input."<br>";
  */
  return $res;
}

function validPage($page)
{
    if(!ereg("^[_/\.A-Za-z0-9\-]+$",$page))
    	return false;
    return file_exists($page);
}

function checkLoginPassword($login,$password)
{
	$requete = "SELECT * FROM user WHERE Login='".$login."';";
	$resultat = exec_game_db_requete($requete);

	if ($ligne = mysql_fetch_array($resultat)) 
	{
		$db_login = $ligne[1];
		$db_password_crypted = $ligne[2];
		if(crypt($password, $db_password_crypted) == $db_password_crypted)
			return true;
	}
	return false; 
}

?>
