
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
                        <td class="cattitle">{L_AVATAR_GALLERY}</td>
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
      <td width="100%" class="genBox">
        <table width="100%" border="0" cellspacing="0" cellpadding="0">

	<tr>
		<td width="0%" class="mainboxLeft"><img src="templates/Cobalt/images/spacer.gif" width="6" height="6"></td>
                	<td width="100%" class="genBox">



              <table width="100%" border="0" cellspacing="0" cellpadding="0">
                <tr>
                  <td class="cBoxStart">

							<table border="0" cellpadding="3" cellspacing="1" width="100%" class="forumline">
							  <tr>
								<td class="avatarcatselect" align="center" valign="middle" colspan="6" height="28"><span class="genmed">{L_CATEGORY}:&nbsp;{S_CATEGORY_SELECT}&nbsp;
								  <input type="submit" class="liteoption" value="{L_GO}" name="avatargallery" />
								  </span></td>
							  </tr>
							</table>

		<table width="100%">
                      <!-- BEGIN avatar_row -->
                      <tr>
                        <!-- BEGIN avatar_column -->
                        <td class="row1" align="center"><img src="{avatar_row.avatar_column.AVATAR_IMAGE}" alt="{avatar_row.avatar_column.AVATAR_NAME}" title="{avatar_row.avatar_column.AVATAR_NAME}" /></td>
                        <!-- END avatar_column -->
                      </tr>
                      <tr>
                        <!-- BEGIN avatar_option_column -->
                        <td class="row2" align="center">
                          <input type="radio" class="checkbox" name="avatarselect" value="{avatar_row.avatar_option_column.S_OPTIONS_AVATAR}" />
                        </td>
                        <!-- END avatar_option_column -->
                      </tr>
                      <!-- END avatar_row -->
		</table>

		  <br /><br />

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
                  <td class="genBoxStart">				 						
					  
		    		<table width="100%" cellspacing="0">
                      <tr> 
                        <td class="catBottom" colspan="{S_COLSPAN}" align="center" height="28">{S_HIDDEN_FIELDS} 
                          <input type="submit" name="submitavatar" value="{L_SELECT_AVATAR}" class="mainoption" />
                          &nbsp;&nbsp; 
                          <input type="submit" name="cancelavatar" value="{L_RETURN_PROFILE}" class="liteoption" />
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

{S_HIDDEN_FIELDS}</form>
