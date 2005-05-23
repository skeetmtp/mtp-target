

<div id="delete">
<center><h1><blink><font color=\"#FF0000\">WARNING</font></blink></h1><br/>
Be sure to know what you do, <u><?php echo CUser::instance()->login(); ?></u> account will be deleted, there is NO way to restore it after that
</center>
</div>

 <table border="1" cellpadding="0" cellspacing="0" bgcolor="#666666">
 <tr>
 <td align="right">
 <table border="0" cellpadding="0" cellspacing="5" bgcolor="#FFFAEA">
 <?php  echo '<form method="post" action="?page=user_merge_delete" name="loginForm">'; ?>
 <!-- <form method="post" action="user_login.php" name="loginForm"> -->  
 <tr>
 <td align="right">
 merge&nbsp;with&nbsp;:
 </td>
 <td colspan="2">
 <input type="text" name="user_merge_login" size="16"/>
 </td>
 </tr>
 <tr>
 <td  align="right">
 pass&nbsp;:
 </td>
 <td colspan="2">
 <input type="password" name="user_merge_password" size="16"/>
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
 <input type="submit" value="merge" tabindex="14" />
 </td>
 </tr>
 </form>
 </table>
 </td>
 </tr>
 </table>


<?php



?>
