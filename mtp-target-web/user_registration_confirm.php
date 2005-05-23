<?php  

/*
printf("login = %s<br/>",$user_register_login);
printf("pass  = %s<br/>",$user_register_password);
*/

if($user_logged)
{
	exit();
}

function rand64()
{
	return floor(64.0*rand()/(getrandmax()+1.0));
}

function cryptPassword($password)
{
	$SaltString = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789./";

	$Salt = "";
	$Salt .= $SaltString[rand64()];
	$Salt .= $SaltString[rand64()];

	return crypt($password, $Salt);
}


if(!isset($user_register_login) || !validInput($user_register_login))
{
	printf("Error in login, choose another one");
	exit();
}
if(!isset ($user_register_password) || !validInput($user_register_password))
{
	printf("Error in password, choose another one");
	exit();
}

if(userNameExist($user_register_login))
{
	printf("Error : login still used, choose another one");
	exit();
}

	$cpassword=cryptPassword($user_register_password);
	printf("%s =>%s<br/>",$user_register_password,$cpassword);

	//reason = sqlQuery("insert into user (Login, Password, Registered) values ('"+login+"', '"+$cpassword+"', NOW())", nbrow, row, result);
	$query = "INSERT INTO user (Login, Password, Registered) values ('$user_register_login', '$cpassword', NOW())";
	$result = exec_game_db_requete($query);
	printf("%s account successfully created<br/>",$user_register_login);
	
?>
