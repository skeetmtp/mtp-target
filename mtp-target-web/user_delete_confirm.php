<?php  

if(!$user_logged)
{
	printf("Error during login confirmation ");
	exit();
}

	/*
	printf("delete login = %s<br>",$user_login);
	printf("delete pass  = %s<br>",$user_password);
	*/
	$delete_uid = userName2Uid($user_login);
	
	//printf("delete uid   = %s<br>",$delete_uid);
	
	$requete = sprintf("DELETE FROM user_session WHERE UId=%d;",$delete_uid);
	$result=exec_game_db_requete($requete);

	$requete = sprintf("DELETE FROM user WHERE UId=%d;",$delete_uid);
	$result=exec_game_db_requete($requete);

?>
