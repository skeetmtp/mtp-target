
<script language="javascript" type="text/javascript">
<!--
function refresh_username(selected_username)
{
	opener.document.forms['post'].username.value = selected_username;
	opener.focus();
	window.close();
}
//-->
</script>

<form method="post" name="search" action="{S_SEARCH_ACTION}">

  <table border="0" cellspacing="0" cellpadding="0" width="100%">
    <tr> 
      <td width="0%" class="mainboxLefttop"><img src="templates/Cobalt/images/spacer.gif" width="6" height="6"></td>
      <td width="100%" class="mainboxTop"><img src="templates/Cobalt/images/spacer.gif" width="6" height="6"></td>
      <td width="0%" class="mainboxRighttop"><img src="templates/Cobalt/images/spacer.gif" width="6" height="6"></td>
    </tr>
    <tr> 
      <td width="0%" class="mainboxLeft"><img src="templates/Cobalt/images/spacer.gif" width="6" height="6"></td>
      <td width="100%" class="genBox"> 
        <table border="0" cellspacing="0" cellpadding="0" width="100%">
          <tr> 
            <td class="genBoxStart">
              <table width="100%" border="0" cellpadding="0" cellspacing="0">
                <tr> 
					<th class="thHead" height="25">{L_SEARCH_USERNAME}</th>
				</tr>
              </table>
            </td>
          </tr>
        </table>
      </td>
      <td width="0%" class="mainboxRight"><img src="templates/Cobalt/images/spacer.gif" width="6" height="6"></td>
    </tr>
    <tr> 
      <td width="0%" class="mainboxMiddleleft"><img src="templates/Cobalt/images/spacer.gif" width="6" height="6"></td>
      <td width="100%" class="mainboxMiddlecenter"><img src="templates/Cobalt/images/spacer.gif" width="6" height="6"></td>
      <td width="0%" class="mainboxMiddleright"><img src="templates/Cobalt/images/spacer.gif" width="6" height="6"></td>
    </tr>
    <tr> 
      <td width="0%" class="mainboxLeft"><img src="templates/Cobalt/images/spacer.gif" width="6" height="6"></td>
      <td width="100%" class="genBox"> 
        <table border="0" cellspacing="0" cellpadding="0" width="100%">
          <tr> 
            <td class="genBoxStart">
              <table width="100%" class="forumline" cellpadding="4" cellspacing="1" border="0">
                <tr> 
                  <td valign="top" class="row1"><span class="genmed"><br />
                    <input type="text" name="search_username" value="{USERNAME}" class="post" />
                    &nbsp; 
                    <input type="submit" name="search" value="{L_SEARCH}" class="liteoption" />
                    </span><br />
                    <span class="gensmall">{L_SEARCH_EXPLAIN}</span><br />
                    <!-- BEGIN switch_select_name -->
                    <span class="genmed">{L_UPDATE_USERNAME}<br />
                    <select name="username_list">{S_USERNAME_OPTIONS}
                    </select>
                    &nbsp; 
                    <input type="submit" class="liteoption" onClick="refresh_username(this.form.username_list.options[this.form.username_list.selectedIndex].value);return false;" name="use" value="{L_SELECT}" />
                    </span><br />
                    <!-- END switch_select_name -->
                    <br />
                    <span class="genmed"><a href="javascript:window.close();" class="genmed">{L_CLOSE_WINDOW}</a></span></td>
                </tr>
              </table>
            </td>
          </tr>
        </table>
      </td>
      <td width="0%" class="mainboxRight"><img src="templates/Cobalt/images/spacer.gif" width="6" height="6"></td>
    </tr>
    <tr> 
      <td width="0%" class="mainboxLeftbottom"><img src="templates/Cobalt/images/spacer.gif" width="6" height="6"></td>
      <td width="100%" valign="top" class="mainboxBottom"><img src="templates/Cobalt/images/spacer.gif" width="6" height="6"></td>
      <td width="0%" class="mainboxRightbottom"><img src="templates/Cobalt/images/spacer.gif" width="6" height="6"></td>
    </tr>
  </table>

{S_HIDDEN_FIELDS}</form>
