 
<form action="{S_LOGIN_ACTION}" method="post" target="_top">

<table width="100%" cellspacing="2" cellpadding="2" border="0" align="center">
  <tr> 
	<td align="left" class="nav"><a href="{U_INDEX}" class="nav">{L_INDEX}</a></td>
  </tr>
</table>

<table width="100%" border="0" cellspacing="0" cellpadding="0">
  <tr> 
    <td colspan="3"> 
      <table width="100%" border="0" cellspacing="0" cellpadding="0">
        <tr> 
          <td width="0%"><img src="templates/Cobalt/images/cat_lcap_whosonline.gif" width="22" height="51"></td>
          <td width="100%" background="templates/Cobalt/images/cat_bar.jpg" valign="top"> 
            <table width="100%" border="0" cellspacing="0" cellpadding="0" height="100%">
              <tr> 
                <td class="cBarStart" valign="top"> 
                  <table border="0" cellspacing="0" cellpadding="0">
                    <tr> 
                      <td valign="top"><img src="templates/Cobalt/images/whosonline_item.gif" width="21" height="39"></td>
                      <td class="cattitle"><span class="cattitle">{L_ENTER_PASSWORD}</span></td>
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
          <td width="0%" class="cboxLeft"><img src="templates/Cobalt/images/spacer.gif" width="6" height="5"></td>
          <td width="100%" class="cbox">
		    
			<table width="100%" border="0" cellspacing="0" cellpadding="0">
              <tr> 
                <td class="cBoxStart"> 
				  
				  <table width="100%" cellpadding="4" cellspacing="1" border="0" align="center">
					<tr> 
					  <td class="row1"> 
						
						<table border="0" cellpadding="3" cellspacing="1" width="100%">
						  <tr> 
							<td width="45%" align="right"><span class="gen">{L_USERNAME}:</span></td>
							<td> 
							  <input type="text" name="username" size="25" maxlength="40" value="{USERNAME}" />
							</td>
						  </tr>
						  <tr> 
							<td align="right"><span class="gen">{L_PASSWORD}:</span></td>
							<td> 
							  <input type="password" name="password" size="25" maxlength="25" />
							</td>
						  </tr>
						  <tr align="center"> 
							<td colspan="2"><span class="gen">{L_AUTO_LOGIN}: 
							  <input type="checkbox" class="checkbox" name="autologin" />
							  </span></td>
						  </tr>
						  <tr align="center"> 
							<td colspan="2">{S_HIDDEN_FIELDS} 
							  <input type="submit" name="login" class="mainoption" value="{L_LOGIN}" />
							</td>
						  </tr>
						  <tr align="center"> 
							<td colspan="2"><span class="genmed"><a href="{U_SEND_PASSWORD}" class="genmed">{L_SEND_PASSWORD}</a></span></td>
						  </tr>
						</table>
						
					  </td>
					</tr>
				  </table>
				  
					</td>
				  </tr>
				</table>
			  
            </td>
          <td width="0%" class="cboxRight"><img src="templates/Cobalt/images/spacer.gif" width="6" height="6"></td>
        </tr>
        <tr> 
          <td width="0%" class="cboxLeftbottom"><img src="templates/Cobalt/images/spacer.gif" width="6" height="6"></td>
          <td width="100%" valign="top" class="cboxBottom"><img src="templates/Cobalt/images/spacer.gif" width="6" height="6"></td>
          <td width="0%" class="cboxRightbottom"><img src="templates/Cobalt/images/spacer.gif" width="6" height="6"></td>
        </tr>
      </table>
	  
    </td>
    <td class="catbox_right"><img src="templates/Cobalt/images/spacer.gif" width="27" height="27"></td>
  </tr>
</table>

  </form>
