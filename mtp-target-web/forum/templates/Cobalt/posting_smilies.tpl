
<script language="javascript" type="text/javascript">
<!--
function emoticon(text) {
	text = ' ' + text + ' ';
	if (opener.document.forms['post'].message.createTextRange && opener.document.forms['post'].message.caretPos) {
		var caretPos = opener.document.forms['post'].message.caretPos;
		caretPos.text = caretPos.text.charAt(caretPos.text.length - 1) == ' ' ? text + ' ' : text;
		opener.document.forms['post'].message.focus();
	} else {
	opener.document.forms['post'].message.value  += text;
	opener.document.forms['post'].message.focus();
	}
}
//-->
</script>


<table border="0" cellspacing="0" cellpadding="0" width="100%">
  <tr> 
    <td width="0%" class="mainboxLefttop"><img src="templates/Cobalt/images/spacer.gif" width="6" height="6"></td>
    <td width="100%" class="mainboxTop"><img src="templates/Cobalt/images/spacer.gif" width="6" height="6"></td>
    <td width="0%" class="mainboxRighttop"><img src="templates/Cobalt/images/spacer.gif" width="6" height="6"></td>
  </tr>
  <tr> 
    <td width="0%" class="mainboxLeft"><img src="templates/Cobalt/images/spacer.gif" width="6" height="6"></td>
    <td width="100%" class="ErrorConfirmBox"> 

      <table border="0" cellspacing="0" cellpadding="0" width="100%">
        <tr> 
          <td class="ErrorConfirmBoxStart">
		  
			<table width="100%" border="0" cellspacing="0" cellpadding="4">
				<tr>
					<th class="thHead" height="25">{L_EMOTICONS}</th>
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
    <td width="100%" class="ErrorConfirmBox">
      
      <table border="0" cellspacing="0" cellpadding="0" width="100%">
        <tr> 
          <td class="ErrorConfirmBoxStart">
		  
		  
			  <table width="100%" border="0" cellspacing="1" cellpadding="4">
				<tr>
					<td><table width="100" border="0" cellspacing="0" cellpadding="5">
						<!-- BEGIN smilies_row -->
						<tr align="center" valign="middle"> 
							<!-- BEGIN smilies_col -->
							<td><a href="javascript:emoticon('{smilies_row.smilies_col.SMILEY_CODE}')"><img src="{smilies_row.smilies_col.SMILEY_IMG}" border="0" alt="{smilies_row.smilies_col.SMILEY_DESC}" title="{smilies_row.smilies_col.SMILEY_DESC}" /></a></td>
							<!-- END smilies_col -->
						</tr>
						<!-- END smilies_row -->
						<!-- BEGIN switch_smilies_extra -->
						<tr align="center"> 
							<td colspan="{S_SMILIES_COLSPAN}"><span  class="nav"><a href="{U_MORE_SMILIES}" onclick="open_window('{U_MORE_SMILIES}', 250, 300);return false" target="_smilies" class="nav">{L_MORE_SMILIES}</a></span></td>
						</tr>
						<!-- END switch_smilies_extra -->
					</table></td>
				</tr>
				<tr>
					<td align="center"><br /><span class="genmed"><a href="javascript:window.close();" class="genmed">{L_CLOSE_WINDOW}</a><br /><br /></span></td>
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

