<?php 

// Warning: This script is not safety, It doesn't check if a user is already online and other possibility

	include_once('service_connection.inc');

	include_once('config.inc');

// ---------------------------------------------------------------------------------------- 
// Functions
// ---------------------------------------------------------------------------------------- 

	// $reason contains the reason why the check failed or success
	// return true if the check is ok
	function checkUserValidity ($login, $password, $clientApplication, &$id, &$reason, &$priv)
	{
		global $DBHost, $DBUserName, $DBPassword, $DBName, $AcceptUnknownUser;

		$link = mysql_connect($DBHost, $DBUserName, $DBPassword) or die ("Can't connect to database host:$DBHost user:$DBUserName");
		mysql_select_db ($DBName) or die ("Can't access to the table dbname:$DBName");
		$query = "SELECT * FROM user where Login='$login'";
		$result = mysql_query ($query) or die ("Can't execute the query: ".$query);

		if (mysql_num_rows ($result) == 0)
		{
			if ($AcceptUnknownUser)
			{
				// login doesn't exist, create it
				$query = "INSERT INTO user (Login, Password) VALUES ('$login', '$password')";
				$result = mysql_query ($query) or die ("Can't execute the query: ".$query);

				// get the user to have his UId
				$query = "SELECT * FROM user WHERE Login='$login'";
				$result = mysql_query ($query) or die ("Can't execute the query: ".$query);

				if (mysql_num_rows ($result) == 1)
				{
					$reason = "Login '".$login."' was created because it was not found in database (error code 50)";
					$row = mysql_fetch_row ($result);
					$id = $row[0];
					$priv = $row[5];

					// add the default permission
					$query = "INSERT INTO permission (UId) VALUES ('$id')";
					$result = mysql_query ($query) or die ("Can't execute the query: ".$query);

					$res = true;
				}
				else
				{
					$reason = "Can't fetch the login '".$login."' (error code 51)";
					$res = false;
				}
			}
			else
			{
				$reason = "Unknown login '".$login."' (error code 52)";
				$res = false;
			}
		}
		else
		{
			$row = mysql_fetch_row ($result);
			$salt = substr($row[2],0,2);
			if ($row[2] == crypt($password, $salt))
			{
				// check if the user can use this application

				$query = "SELECT * FROM permission WHERE UId='$row[0]' AND ClientApplication='$clientApplication'";
				$result = mysql_query ($query) or die ("Can't execute the query: ".$query);
				if (mysql_num_rows ($result) == 0)
				{
					// no permission
					$reason = "You can't use the client application '$clientApplication' (error code 53)";
					$res = false;
				}
				else
				{
					// check if the user not already online

					if ($row[4] != "Offline")
					{
						$reason = "$login is already online and ";
						// ask the LS to remove the client
						if (disconnectClient ($row[3], $row[0], $tempres))
						{
							$reason =  $reason."was just disconnected. Now you can retry the identification (error code 54)";

							$query = "update shard set NbPlayers=NbPlayers-1 where ShardId=$row[3]";
							$result = mysql_query ($query) or die ("Can't execute the query: '$query' errno:".mysql_errno().": ".mysql_error());

							$query = "update user set ShardId=-1, State='Offline' where UId=$row[0]";
							$result = mysql_query ($query) or die ("Can't execute the query: '$query' errno:".mysql_errno().": ".mysql_error());
						}
						else
						{
							$reason = $reason."can't be disconnected: $tempres (error code 55)";
						}
						$res = false;
					}
					else
					{
						$id = $row[0];
						$priv = $row[5];
						$res = true;
					}
				}
			}
			else
			{
				$reason = "Bad password (error code 56)";
				$res = false;
			}
		}
		mysql_close($link);
		return $res;
	}

	function displayAvailableShards($id, $clientApplication)
	{
		global $PHP_SELF;
		global $DBHost, $DBUserName, $DBPassword, $DBName;

		$link = mysql_connect($DBHost, $DBUserName, $DBPassword) or die ("0:Can't connect to database host:$DBHost user:$DBUserName");
		mysql_select_db ($DBName) or die ("0:Can't access to the table dbname:$DBName");
		$query = "SELECT * FROM shard WHERE ClientApplication='".$clientApplication."'";
		$result = mysql_query ($query) or die ("0:Can't execute the query: ".$query);
		
		$nbs = 0;
		$res = "";
		if (mysql_num_rows ($result) > 0)
		{
			//echo "<h1>Please, select a shard:</h1>\n";
			while($row = mysql_fetch_array($result))
			{
				$query2 = "SELECT * FROM permission WHERE UId='".$id."' AND ClientApplication='".$clientApplication."' AND ShardId='".$row["ShardId"]."'";
				$result2 = mysql_query ($query2) or die ("Can't execute the query: ".$query2);

				// only display the shard if the user have the good application name AND access to this shard with the permission table
				if (mysql_num_rows ($result2) > 0 && $row["ProgramName"] == $programName)
				{
					$nbs++;
					$res = $res.$row["Version"]."|";
					$res = $res.$row["Online"]."|";
					$res = $res.$row["ShardId"]."|";
					$res = $res.$row["Name"]."|";
					$res = $res."999999|";
					$res = $res.$row["WsAddr"]."|";
					$res = $res.$row["PatchURL"];
					$res = $res."\n";
				}
			}
		}

		echo "1:".$nbs."\n";
		echo $res;
		mysql_close($link);
		return $res;
	}

// --------------------------------------------------------------------------------------
// main 
// --------------------------------------------------------------------------------------

	if (!checkUserValidity($login, $password, $clientApplication, $id, $reason, $priv))
	{
		echo "0:".$reason;
	}
	else
	{
		if ($cmd == "login")
		{
			// user selected a shard, try to add the user to the shard

			if (askClientConnection($shardid, $id, $login, $priv, $res))
			{
				// access granted, send cookie and addr
				echo "1:".$res;
			}
			else
			{
				// access denied, display why
				echo "0:".$res;
			}
		}
		else
		{
			// user logged, display the available shard
			displayAvailableShards ($id, $clientApplication);
		}
	}
?>
