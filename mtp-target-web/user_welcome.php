 <table border="1" cellpadding="0" cellspacing="0" bgcolor="#666666">
 <tr>
 <td align="right">
 <table border="0" cellpadding="0" cellspacing="5" bgcolor="#FFFAEA">
 <tr>
 <td>
 <?php
   printf("Welcome <b>%s</b>... (<a href=\"user_logout.php\">logout</a>)<br>",$user_login);
   printf("Today score : %d<br>",userTodayScore($user_id));
   printf("<b>");
   printf("<a href=\"?page=user_settings.php\">your settings</a><br>");
   printf("<a href=\"?page=stat_user.php&p_user_id=%d\">your stats</a><br>",$user_id);
   printf("</b>");
 ?>
 </td>
 </tr>
 </table>
 </td>
 </tr>
 </table>

