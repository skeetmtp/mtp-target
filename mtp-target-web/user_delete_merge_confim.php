<?php  
/*
printf("login = %s<br/>",$user_merge_login);
printf("pass  = %s<br/>",$user_merge_password);
*/

if(!$user_logged)
{
	printf("Error during login confirmation ");
	exit();
}

if(!(isset($user_merge_login) && validInput($user_merge_login) && isset ($user_merge_password) && validInput($user_merge_password)) )
{
	printf("Error in login or password");
	exit();
}

if(!checkLoginPassword($user_merge_login,$user_merge_password))
{
	printf("Error in login or password");
	exit();
}

	$delete_uid = CUser::instance()->uid();//userName2Uid($user_login);
	$merge_uid = userName2Uid($user_merge_login);


	/*
	printf("merge login  = %s<br/>",$user_merge_login);
	printf("merge pass   = %s<br/>",$user_merge_password);
	printf("delete login = %s<br/>",$user_login);
	printf("delete pass  = %s<br/>",$user_password);
	printf("delete uid   = %s<br/>",$delete_uid);
	printf("merge uid    = %s<br/>",$merge_uid);
	*/
	
	
	$requete = sprintf("UPDATE user_session SET UId=%d WHERE UId=%d;",$merge_uid,$delete_uid);
	$result=exec_game_db_requete($requete);

	$oldScore = 0;
	$requete = sprintf("Select Score FROM user WHERE UId=%d;",$delete_uid);
	$result=exec_game_db_requete($requete);
	if($ligne = mysql_fetch_array($result))
	{
		$oldScore = $ligne[0];
	}

	$requete = sprintf("UPDATE user SET Score=Score+%d WHERE UId=%d;",$oldScore,$merge_uid);
	$result=exec_game_db_requete($requete);

	$requete = sprintf("DELETE FROM user WHERE UId=%d;",$delete_uid);
	$result=exec_game_db_requete($requete);

	printf("%s merged successfully with %s<br/>",CUser::instance()->login(),$user_merge_login);
	
?>
