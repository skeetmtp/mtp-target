<?php  
include_once("stat_function.php");

if(!$user_logged)
{
	printf("Error during login confirmation ");
	exit();
}

if(!isset($user_newname_login) || !validInput($user_newname_login))
{
	printf("Error new name not valid ");
	exit();
}


if(userNameExist($user_newname_login))
{
	printf("User %s already exists.",$user_newname_login);
	return;
}
else
{
	$uid = userName2Uid($user_login);
	$requete = sprintf("UPDATE user SET Login='%s' WHERE UId=%d;",$user_newname_login,$uid);
	$result=exec_game_db_requete($requete);

	$requete = sprintf("UPDATE game_user SET Login='%s' WHERE UId=%d;",$user_newname_login,$uid);
	$result=exec_requete($requete);

	@unlink($cache_dir."/user_welcome_en_".$uid.".html");
	@unlink($cache_dir."/user_welcome_fr_".$uid.".html");

	printf("%s successfully rennamed to %s<br>",$user_login,$user_newname_login);
	
}
	
	

?>
