
<form action="{S_PROFILE_ACTION}" {S_FORM_ENCTYPE} method="post">

{ERROR_BOX}

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
                        <td class="cattitle">{L_REGISTRATION_INFO}</td>
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
                	<!-- BEGIN switch_namechange_disallowed -->
                	<tr> 
                		<td class="row1" width="38%"><span class="genmed"><strong>{L_USERNAME}: *</strong></span></td>
                		<td class="row2"><input type="hidden" name="username" value="{USERNAME}" /><span class="gen"><b>{USERNAME}</b></span></td>
                	</tr>
                	<!-- END switch_namechange_disallowed -->
                	<!-- BEGIN switch_namechange_allowed -->
                	<tr> 
                		<td class="row1" width="38%"><span class="genmed"><strong>{L_USERNAME}: *</strong></span></td>
                		<td class="row2"><input type="text" class="post" style="width:200px" name="username" size="25" maxlength="25" value="{USERNAME}" /></td>
                	</tr>
                	<!-- END switch_namechange_allowed -->
                      <tr>
                        <td class="row1"><span class="genmed"><strong>{L_EMAIL_ADDRESS}: *</strong></span></td>
                        <td class="row2">
                          <input type="text" class="post" style="width:200px" name="email" size="25" maxlength="255" value="{EMAIL}" />
                        </td>
                      </tr>
                      <!-- BEGIN switch_edit_profile -->
                      <tr> 
                        <td class="row1"><span class="genmed"><strong>{L_CURRENT_PASSWORD}: *<strong></strong></span><br />
                          <span class="gensmall">{L_CONFIRM_PASSWORD_EXPLAIN}</span></td>
                        <td class="row2"> 
                          <input type="password" class="post" style="width: 200px" name="cur_password" size="25" maxlength="32" value="{CUR_PASSWORD}" />
                        </td>
                      </tr>
                      <!-- END switch_edit_profile -->
                      <tr> 
                        <td class="row1"><span class="genmed"><strong>{L_NEW_PASSWORD}: *</strong></span><br />
                          <span class="gensmall">{L_PASSWORD_IF_CHANGED}</span></td>
                        <td class="row2"> 
                          <input type="password" class="post" style="width: 200px" name="new_password" size="25" maxlength="32" value="{NEW_PASSWORD}" />
                        </td>
                      </tr>
                	<tr> 
                	  <td class="row1"><span class="genmed"><strong>{L_CONFIRM_PASSWORD}: *</strong></span><br />
                		<span class="gensmall">{L_PASSWORD_CONFIRM_IF_CHANGED}</span></td>
                	  <td class="row2">
                		<input type="password" class="post" style="width: 200px" name="password_confirm" size="25" maxlength="32" value="{PASSWORD_CONFIRM}" />
                	  </td>
                	</tr>
                	<!-- Visual Confirmation -->
                	<!-- BEGIN switch_confirm -->
                	<tr>
                		<td class="row1" colspan="2" align="center"><span class="gensmall">{L_CONFIRM_CODE_IMPAIRED}</span><br /><br />{CONFIRM_IMG}<br /><br /></td>
                	</tr>
                	<tr> 
                	  <td class="row1"><span class="genmed"><strong>{L_CONFIRM_CODE}: *</strong></span><br /><span class="gensmall">{L_CONFIRM_CODE_EXPLAIN}</span></td>
                	  <td class="row2"><input type="text" class="post" style="width: 200px" name="confirm_code" size="6" maxlength="6" value="" /></td>
                	</tr>
                	<!-- END switch_confirm -->
                      <tr> 
                        <td colspan="2" height="28">&nbsp;</td>
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
    <td width="100%" class="mainBox" cellpadding="0">

                <table cellspacing="0" width="100%">
			<tr>
				<th class="thHead">{L_PROFILE_INFO}</th>
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
    <td width="100%" class="cBox" cellpadding="0">
              
              <table width="100%" border="0" cellspacing="0" cellpadding="0">
                <tr>
                  <td class="cBoxStart" align="center">

                  <table>
                      <tr>
                        <td class="row2" colspan="2"><span class="gensmall">{L_PROFILE_INFO_NOTICE}</span></td>
                      </tr>
                      <tr> 
                        <td class="row1" width=38%"><span class="genmed"><strong>{L_ICQ_NUMBER}:</strong></span></td>
                        <td class="row2" width=62%">
                          <input type="text" name="icq" class="post"style="width: 100px"  size="10" maxlength="15" value="{ICQ}" />
                        </td>
                      </tr>
                      <tr> 
                        <td class="row1"><span class="genmed"><strong>{L_AIM}:</strong></span></td>
                        <td class="row2"> 
                          <input type="text" class="post"style="width: 150px"  name="aim" size="20" maxlength="255" value="{AIM}" />
                        </td>
                      </tr>
                      <tr> 
                        <td class="row1"><span class="genmed"><strong>{L_MESSENGER}:</strong></span></td>
                        <td class="row2"> 
                          <input type="text" class="post"style="width: 150px"  name="msn" size="20" maxlength="255" value="{MSN}" />
                        </td>
                      </tr>
                      <tr> 
                        <td class="row1"><span class="genmed"><strong>{L_YAHOO}:</strong></span></td>
                        <td class="row2"> 
                          <input type="text" class="post"style="width: 150px"  name="yim" size="20" maxlength="255" value="{YIM}" />
                        </td>
                      </tr>
                      <tr> 
                        <td class="row1"><span class="genmed"><strong>{L_WEBSITE}:</strong></span></td>
                        <td class="row2"> 
                          <input type="text" class="post"style="width: 200px"  name="website" size="25" maxlength="255" value="{WEBSITE}" />
                        </td>
                      </tr>
                      <tr> 
                        <td class="row1"><span class="genmed"><strong>{L_LOCATION}:</strong></span></td>
                        <td class="row2"> 
                          <input type="text" class="post"style="width: 200px"  name="location" size="25" maxlength="100" value="{LOCATION}" />
                        </td>
                      </tr>
                      <tr> 
                        <td class="row1"><span class="genmed"><strong>{L_OCCUPATION}:</strong></span></td>
                        <td class="row2"> 
                          <input type="text" class="post"style="width: 200px"  name="occupation" size="25" maxlength="100" value="{OCCUPATION}" />
                        </td>
                      </tr>
                      <tr> 
                        <td class="row1"><span class="genmed"><strong>{L_INTERESTS}:</strong></span></td>
                        <td class="row2"> 
                          <input type="text" class="post"style="width: 200px"  name="interests" size="35" maxlength="150" value="{INTERESTS}" />
                        </td>
                      </tr>
                      <tr> 
                        <td class="row1"><span class="genmed"><strong>{L_SIGNATURE}:</strong></span><br />
                          <span class="gensmall">{L_SIGNATURE_EXPLAIN}<br />
                          <br />
                          {HTML_STATUS}<br />
                          {BBCODE_STATUS}<br />
                          {SMILIES_STATUS}</span></td>
                        <td class="row2"> 
                          <textarea name="signature"style="width: 300px"  rows="6" cols="30" class="post">{SIGNATURE}</textarea>
                        </td>
                      </tr>
                      <tr> 
                        <td colspan="2" height="28">&nbsp;</td>
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
    <td width="100%" class="mainBox" cellpadding="0">

                <table cellspacing="0" width="100%">
			<tr>
				<th class="thHead">{L_PREFERENCES}</th>
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
    <td width="100%" class="cBox" cellpadding="0">
              
              <table width="100%" border="0" cellspacing="0" cellpadding="0">
                <tr>
                  <td class="cBoxStart" align="center">

                    <table width="100%">
                      <tr>
                        <td class="row1" width="38%"><span class="genmed"><strong>{L_PUBLIC_VIEW_EMAIL}:</strong></span></td>
                        <td class="row2" width="62%">
                          <input type="radio" class="checkbox" name="viewemail" value="1" {VIEW_EMAIL_YES} />
                          <span class="gen">{L_YES}</span>&nbsp;&nbsp; 
                          <input type="radio" class="checkbox" name="viewemail" value="0" {VIEW_EMAIL_NO} />
                          <span class="gen">{L_NO}</span></td>
                      </tr>
                      <tr> 
                        <td class="row1"><span class="genmed"><strong>{L_HIDE_USER}:</strong></span></td>
                        <td class="row2"> 
                          <input type="radio" class="checkbox" name="hideonline" value="1" {HIDE_USER_YES} />
                          <span class="gen">{L_YES}</span>&nbsp;&nbsp; 
                          <input type="radio" class="checkbox" name="hideonline" value="0" {HIDE_USER_NO} />
                          <span class="gen">{L_NO}</span></td>
                      </tr>
                      <tr> 
                        <td class="row1"><span class="genmed"><strong>{L_NOTIFY_ON_REPLY}:</strong></span><br />
                          <span class="gensmall">{L_NOTIFY_ON_REPLY_EXPLAIN}</span></td>
                        <td class="row2"> 
                          <input type="radio" class="checkbox" name="notifyreply" value="1" {NOTIFY_REPLY_YES} />
                          <span class="gen">{L_YES}</span>&nbsp;&nbsp; 
                          <input type="radio" class="checkbox" name="notifyreply" value="0" {NOTIFY_REPLY_NO} />
                          <span class="gen">{L_NO}</span></td>
                      </tr>
                      <tr> 
                        <td class="row1"><span class="genmed"><strong>{L_NOTIFY_ON_PRIVMSG}:</strong></span></td>
                        <td class="row2"> 
                          <input type="radio" class="checkbox" name="notifypm" value="1" {NOTIFY_PM_YES} />
                          <span class="gen">{L_YES}</span>&nbsp;&nbsp; 
                          <input type="radio" class="checkbox" name="notifypm" value="0" {NOTIFY_PM_NO} />
                          <span class="gen">{L_NO}</span></td>
                      </tr>
                      <tr> 
                        <td class="row1"><span class="genmed"><strong>{L_POPUP_ON_PRIVMSG}:</strong></span><br />
                          <span class="gensmall">{L_POPUP_ON_PRIVMSG_EXPLAIN}</span></td>
                        <td class="row2"> 
                          <input type="radio" class="checkbox" name="popup_pm" value="1" {POPUP_PM_YES} />
                          <span class="gen">{L_YES}</span>&nbsp;&nbsp; 
                          <input type="radio" class="checkbox" name="popup_pm" value="0" {POPUP_PM_NO} />
                          <span class="gen">{L_NO}</span></td>
                      </tr>
                      <tr> 
                        <td class="row1"><span class="genmed"><strong>{L_ALWAYS_ADD_SIGNATURE}:</strong></span></td>
                        <td class="row2"> 
                          <input type="radio" class="checkbox" name="attachsig" value="1" {ALWAYS_ADD_SIGNATURE_YES} />
                          <span class="gen">{L_YES}</span>&nbsp;&nbsp; 
                          <input type="radio" class="checkbox" name="attachsig" value="0" {ALWAYS_ADD_SIGNATURE_NO} />
                          <span class="gen">{L_NO}</span></td>
                      </tr>
                      <tr> 
                        <td class="row1"><span class="genmed"><strong>{L_ALWAYS_ALLOW_BBCODE}:</strong></span></td>
                        <td class="row2"> 
                          <input type="radio" class="checkbox" name="allowbbcode" value="1" {ALWAYS_ALLOW_BBCODE_YES} />
                          <span class="gen">{L_YES}</span>&nbsp;&nbsp; 
                          <input type="radio" class="checkbox" name="allowbbcode" value="0" {ALWAYS_ALLOW_BBCODE_NO} />
                          <span class="gen">{L_NO}</span></td>
                      </tr>
                      <tr> 
                        <td class="row1"><span class="genmed"><strong>{L_ALWAYS_ALLOW_HTML}:</strong></span></td>
                        <td class="row2"> 
                          <input type="radio" class="checkbox" name="allowhtml" value="1" {ALWAYS_ALLOW_HTML_YES} />
                          <span class="gen">{L_YES}</span>&nbsp;&nbsp; 
                          <input type="radio" class="checkbox" name="allowhtml" value="0" {ALWAYS_ALLOW_HTML_NO} />
                          <span class="gen">{L_NO}</span></td>
                      </tr>
                      <tr> 
                        <td class="row1"><span class="genmed"><strong>{L_ALWAYS_ALLOW_SMILIES}:</strong></span></td>
                        <td class="row2"> 
                          <input type="radio" class="checkbox" name="allowsmilies" value="1" {ALWAYS_ALLOW_SMILIES_YES} />
                          <span class="gen">{L_YES}</span>&nbsp;&nbsp; 
                          <input type="radio" class="checkbox" name="allowsmilies" value="0" {ALWAYS_ALLOW_SMILIES_NO} />
                          <span class="gen">{L_NO}</span></td>
                      </tr>
                      <tr> 
                        <td class="row1"><span class="genmed"><strong>{L_BOARD_LANGUAGE}:</strong></span></td>
                        <td class="row2"><span class="gensmall">{LANGUAGE_SELECT}</span></td>
                      </tr>
                      <tr>
                        <td class="row1"><span class="genmed"><strong>{L_BOARD_STYLE}:</strong></span></td>
                        <td class="row2"><span class="gensmall">{STYLE_SELECT}</span></td>
                      </tr>
                      <tr> 
                        <td class="row1"><span class="genmed"><strong>{L_TIMEZONE}:</strong></span></td>
                        <td class="row2"><span class="gensmall">{TIMEZONE_SELECT}</span></td>
                      </tr>
                      <tr> 
                        <td class="row1"><span class="genmed"><strong>{L_DATE_FORMAT}:</strong></span><br />
                          <span class="gensmall">{L_DATE_FORMAT_EXPLAIN}</span></td>
                        <td class="row2"> 
                          <input type="text" name="dateformat" value="{DATE_FORMAT}" maxlength="14" class="post" />
                        </td>
                      </tr>
                      <!-- BEGIN switch_avatar_block -->
                      <tr> 
                        <td colspan="2" height="28">&nbsp;</td>
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
    <td width="100%" class="mainBox" cellpadding="0">

                <table cellspacing="0" width="100%">
			<tr>
				<th class="thHead">{L_AVATAR_PANEL}</th>
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
    <td width="100%" class="cBox" cellpadding="0">

              <table width="100%" border="0" cellspacing="0" cellpadding="0">
                <tr>
                  <td class="cBoxStart" align="center">

                    <table>
                      <tr>
                        <td class="row1" colspan="2" style="padding-top: 20px;">
						 
                          <table width="70%" cellspacing="2" cellpadding="0" border="0" align="center">
                            <tr> 
                              <td width="65%"><span class="gensmall">{L_AVATAR_EXPLAIN}</span></td>
                              <td align="center"><span class="gensmall">{L_CURRENT_IMAGE}</span><br />
                                {AVATAR}<br />
                                <input type="checkbox" class="checkbox" name="avatardel" />
                                &nbsp;<span class="gensmall">{L_DELETE_AVATAR}</span></td>
                            </tr>
							<tr>
								<td>&nbsp;</td>
							</tr>
                          </table>
						  
                        </td>
                      </tr>
                      <!-- BEGIN switch_avatar_local_upload -->
                      <tr> 
                        <td class="row1" width="38%"><span class="genmed"><strong>{L_UPLOAD_AVATAR_FILE}:</strong></span></td>
                        <td class="row2"> 
                          <input type="hidden" name="MAX_FILE_SIZE" value="{AVATAR_SIZE}" />
                          <input type="file" name="avatar" class="post" />
                        </td>
                      </tr>
                      <!-- END switch_avatar_local_upload -->
                      <!-- BEGIN switch_avatar_remote_upload -->
                      <tr> 
                        <td class="row1" width="38%"><span class="genmed"><strong>{L_UPLOAD_AVATAR_URL}:</strong></span><br />
                          <span class="gensmall">{L_UPLOAD_AVATAR_URL_EXPLAIN}</span></td>
                        <td class="row2" width="62%"> 
                          <input type="text" name="avatarurl" size="40" class="post" style="width:200px" />
                        </td>
                      </tr>
                      <!-- END switch_avatar_remote_upload -->
                      <!-- BEGIN switch_avatar_remote_link -->
                      <tr> 
                        <td class="row1" width="38%"><span class="genmed"><strong>{L_LINK_REMOTE_AVATAR}:</strong></span><br />
                          <span class="gensmall">{L_LINK_REMOTE_AVATAR_EXPLAIN}</span></td>
                        <td class="row2" width="62%"> 
                          <input type="text" name="avatarremoteurl" size="40" class="post" style="width:200px" />
                        </td>
                      </tr>
                      <!-- END switch_avatar_remote_link -->
                      <!-- BEGIN switch_avatar_local_gallery -->
                      <tr> 
                        <td class="row1" width="38%"><span class="genmed"><strong>{L_AVATAR_GALLERY}:</strong></span></td>
                        <td class="row2" width="62%"> 
                          <input type="submit" name="avatargallery" value="{L_SHOW_GALLERY}" class="liteoption" />
                        </td>
                      </tr>
                      <!-- END switch_avatar_local_gallery -->
                      <!-- END switch_avatar_block -->

                      <tr> 
                        <td colspan="2" height="28">&nbsp;</td>
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
    <td width="100%" class="mainBox" cellpadding="0">

                <table cellspacing="0" width="100%">
                      <tr>
                        <td class="catBottom" colspan="2" align="center" height="28">{S_HIDDEN_FIELDS} 
                          <input type="submit" name="submit" value="{L_SUBMIT}" class="mainoption" />
                          &nbsp;&nbsp; 
                          <input type="reset" value="{L_RESET}" name="reset" class="liteoption" />
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
