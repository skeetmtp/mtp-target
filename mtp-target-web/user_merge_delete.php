<?php  
/*
printf("login = %s<br>",$user_merge_login);
printf("pass  = %s<br>",$user_merge_password);
*/

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

?>

<div id="delete">
<center><h1><blink>WARNING</blink></h1><br>
Be sure to know what you do, <u><?php echo $user_login; ?></u> account will be deleted, there is NO way to restore it after that
</center>
</div>

 <table border="1" cellpadding="0" cellspacing="0" bgcolor="#666666">
 <tr>
 <td align="right">
 <table border="0" cellpadding="0" cellspacing="5" bgcolor="#FFFAEA">
 <?php  echo '<form method="post" action="?page=user_delete_merge_confim.php" name="loginForm">'; ?>
 <!-- <form method="post" action="user_login.php" name="loginForm"> -->  
 <tr>
 <td align="right">
 merge&nbsp;with&nbsp;:
 </td>
 <td colspan="2">
	<?php echo $user_merge_login; ?>
 </td>
 </tr>
 <tr>
 <td align="right">
 delete&nbsp;:
 </td>
 <td colspan="2">
	<?php echo $user_login; ?>
 </td>
 </tr>
 <tr>
 <td  align="right">
 pass&nbsp;:
 </td>
 <td colspan="2">
 <input type="password" name="user_password" size="16"/>
 </td>
 </tr>
 <tr>
 <td align="left">
 &nbsp;
 </td>
 <td>
 &nbsp;
 </td>
 <td  align="right">
<?php
 printf("<input type=\"hidden\" name=\"user_login\" value=\"%s\"/>\n",$user_login);
 printf("<input type=\"hidden\" name=\"user_merge_login\" value=\"%s\"/>\n",$user_merge_login);
 printf("<input type=\"hidden\" name=\"user_merge_password\" value=\"%s\"/>\n",$user_merge_password);
?>
 <input type="submit" value="delete" tabindex="14" />
 </td>
 </tr>
 </form>
 </table>
 </td>
 </tr>
 </table>


<?php



?>
