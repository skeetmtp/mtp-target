
<script language="JavaScript" type="text/javascript">
<!--
function checkForm(formObj) {

	formErrors = false;    

	if (formObj.message.value.length < 2) {
		formErrors = "{L_EMPTY_MESSAGE_EMAIL}";
	}
	else if ( formObj.subject.value.length < 2)
	{
		formErrors = "{L_EMPTY_SUBJECT_EMAIL}";
	}

	if (formErrors) {
		alert(formErrors);
		return false;
	}
}
//-->
</script>

<form action="{S_POST_ACTION}" method="post" name="post" onSubmit="return checkForm(this)">

{ERROR_BOX}

<table width="100%" cellspacing="2" cellpadding="2" border="0" align="center">
	<tr>
		<td align="left"><span  class="nav"><a href="{U_INDEX}" class="nav">{L_INDEX}</a></span></td>
	</tr>
</table>


  <table width="100%" border="0" cellspacing="0" cellpadding="0">
    <tr> 
      <td colspan="3"> 
        <table width="100%" border="0" cellspacing="0" cellpadding="0">
          <tr> 
            <td width="0%"><img src="templates/Cobalt/images/cat_lcap.gif" width="22" height="51"></td>
            <td width="100%" background="templates/Cobalt/images/cat_bar.jpg" valign="top"> 
              <table width="100%" border="0" cellspacing="0" cellpadding="0" height="100%">
                <tr> 
                  <td class="cBarStart" valign="top"> 
                    <table border="0" cellspacing="0" cellpadding="0">
                      <tr> 
                        <td valign="top"><img src="templates/Cobalt/images/cat_arrow.gif" width="25" height="39"></td>
                        <td class="cattitle"><b>{L_SEND_EMAIL_MSG}</b></td>
                      </tr>
                    </table>
                  </td>
                  <td><img src="templates/Cobalt/images/spacer.gif" width="1" height="51"></td>
                </tr>
              </table>
            </td>
            <td width="0%"><img src="templates/Cobalt/images/cat_rcap.gif" width="33" height="51"></td>
          </tr>
        </table>
      </td>
    </tr>
    <tr> 
      <td width="0%"><img src="templates/Cobalt/images/spacer.gif" width="16" height="22"></td>
      <td width="100%"> 
        <table width="100%" border="0" cellspacing="0" cellpadding="0">
          <tr> 
            <td width="0%" class="cboxLeft"><img src="templates/Cobalt/images/spacer.gif" width="6" height="6"></td>
            <td width="100%" class="cbox"> 
              <table width="100%" border="0" cellspacing="0" cellpadding="0">
                <tr> 
                  <td class="cBoxStart">
                    <table border="0" cellpadding="3" cellspacing="1" width="100%" class="forumline">
                      <tr> 
                        <td class="row1" width="22%"><span class="gen"><b>{L_RECIPIENT}</b></span></td>
                        <td class="row2" width="78%"><span class="gen"><b>{USERNAME}</b></span> 
                        </td>
                      </tr>
                      <tr> 
                        <td class="row1" width="22%"><span class="gen"><b>{L_SUBJECT}</b></span></td>
                        <td class="row2" width="78%"><span class="gen">
                          <input type="text" name="subject" size="45" maxlength="100" style="width:450px" tabindex="2" class="post" value="{SUBJECT}" />
                          </span> </td>
                      </tr>
                      <tr> 
                        <td class="row1" valign="top"><span class="gen"><b>{L_MESSAGE_BODY}</b></span><br />
                          <span class="gensmall">{L_MESSAGE_BODY_DESC}</span></td>
                        <td class="row2"><span class="gen">
                          <textarea name="message" rows="25" cols="40" wrap="virtual" style="width:500px" tabindex="3" class="post">{MESSAGE}</textarea>
                          </span></td>
                      </tr>
                      <tr> 
                        <td class="row1" valign="top"><span class="gen"><b>{L_OPTIONS}</b></span></td>
                        <td class="row2">
                          <table cellspacing="0" cellpadding="1" border="0">
                            <tr> 
                              <td>
                                <input type="checkbox" class="checkbox" name="cc_email"  value="1" checked />
                              </td>
                              <td><span class="gen">{L_CC_EMAIL}</span></td>
                            </tr>
                          </table>
                        </td>
                      </tr>
                      <tr> 
						<td class="catBottom" colspan="2" align="center" height="28">{S_HIDDEN_FIELDS}<input type="submit" tabindex="6" name="submit" class="mainoption" value="{L_SEND_EMAIL}" /></td>
                      </tr>
                    </table>
                  </td>
                </tr>
              </table>
            </td>
            <td width="0%" class="cboxRight"><img src="templates/Cobalt/images/spacer.gif" width="6" height="6"></td>
          </tr>
          <tr> 
            <td width="0%" class="cboxLeftbottom">&nbsp;</td>
            <td width="100%" valign="top" class="cboxBottom"><img src="templates/Cobalt/images/spacer.gif" width="6" height="6"></td>
            <td width="0%" class="cboxRightbottom">&nbsp;</td>
          </tr>
        </table>
      </td>
      <td class="catbox_right"><img src="templates/Cobalt/images/spacer.gif" width="27" height="27"></td>
    </tr>
  </table>

  <table width="100%" cellspacing="2" border="0" align="center" cellpadding="2">
	<tr> 
		<td align="right" valign="top"><span class="gensmall">{S_TIMEZONE}</span></td>
	</tr>
</table>{S_HIDDEN_FIELDS}</form>

<table width="100%" cellspacing="2" border="0" align="center">
	<tr>
		<td valign="top" align="right">{JUMPBOX}</td>
	</tr>
</table>
