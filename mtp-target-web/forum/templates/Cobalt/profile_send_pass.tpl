
<form action="{S_PROFILE_ACTION}" method="post">
<table width="100%" cellspacing="2" cellpadding="2" border="0" align="center">
  <tr> 
	<td align="left"><span class="nav"><a href="{U_INDEX}" class="nav">{L_INDEX}</a></span></td>
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
                        <td class="cattitle">{L_SEND_PASSWORD}</td>
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
                        <td class="row2" colspan="2"><span class="genmed">{L_ITEMS_REQUIRED}</span></td>
                      </tr>
                      <tr> 
                        <td class="row1" width="38%"><span class="gen">{L_USERNAME}: 
                          *</span></td>
                        <td class="row2"> 
                          <input type="text" class="post" style="width: 200px" name="username" size="25" maxlength="40" value="{USERNAME}" />
                        </td>
                      </tr>
                      <tr> 
                        <td class="row1"><span class="gen">{L_EMAIL_ADDRESS}: 
                          *</span></td>
                        <td class="row2"> 
                          <input type="text" class="post" style="width: 200px" name="email" size="25" maxlength="255" value="{EMAIL}" />
                        </td>
                      </tr>
					  <tr>
						  <td colspan="2" align="center" height="28">&nbsp;</td>
					  </tr>
					</table>
					
                  </td>
                </tr>
              </table>
			  
            </td>
            <td width="0%" class="cboxRight"><img src="templates/Cobalt/images/spacer.gif" width="6" height="6"></td>
          </tr>
			<tr> 
				<td width="0%" class="mainboxMiddleleft"><img src="templates/Cobalt/images/spacer.gif" width="6" height="6"></td>
				<td width="100%" class="mainboxMiddlecenter"><img src="templates/Cobalt/images/spacer.gif" width="6" height="6"></td>
				<td width="0%" class="mainboxMiddleright"><img src="templates/Cobalt/images/spacer.gif" width="6" height="6"></td>
			</tr>
			
			<tr> 
				<td width="0%" class="mainboxLeft"><img src="templates/Cobalt/images/spacer.gif" width="6" height="6"></td>
				<td width="100%" class="genBox">
				
				  <table width="100%" border="0" cellspacing="0" cellpadding="0">
					<tr> 
					  <td class="cBoxStart">				 		  
						
						<table border="0" cellpadding="3" cellspacing="1" width="100%" class="forumline">										  
						  <tr> 
							<td class="catBottom" colspan="2" align="center" height="28">{S_HIDDEN_FIELDS} 
							  <input type="submit" name="submit" value="{L_SUBMIT}" class="mainoption" />
							  &nbsp;&nbsp; 
							  <input type="reset" value="{L_RESET}" name="reset" class="liteoption" />
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
            <td width="0%" class="cboxLeftbottom">&nbsp;</td>
            <td width="100%" valign="top" class="cboxBottom"><img src="templates/Cobalt/images/spacer.gif" width="6" height="6"></td>
            <td width="0%" class="cboxRightbottom">&nbsp;</td>
          </tr>
        </table>
      </td>
      <td class="catbox_right"><img src="templates/Cobalt/images/spacer.gif" width="27" height="27"></td>
    </tr>
  </table>

  {S_HIDDEN_FIELDS}</form>
