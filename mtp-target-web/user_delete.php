

<div id="delete">
<center><h1><blink>WARNING</blink></h1><br>
Be sure to know what you do, <u><?php echo $user_login; ?></u> account will be deleted, there is NO way to restore it after that
</center>
</div>

 <table border="1" cellpadding="0" cellspacing="0" bgcolor="#666666">
 <tr>
 <td align="right">
 <table border="0" cellpadding="0" cellspacing="5" bgcolor="#FFFAEA">
 <?php  echo '<form method="post" action="?page=user_delete_confirm.php" name="loginForm">'; ?>
 <!-- <form method="post" action="user_login.php" name="loginForm"> -->  
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
