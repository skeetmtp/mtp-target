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
				<th class="thHead" colspan="2">{L_ADD_A_POLL}</th>
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
				<td class="row1" width="22%"><span class="gensmall">&nbsp;</span></td>
                                <td class="row2" width="78%"><span class="gensmall"><br />{L_ADD_POLL_EXPLAIN}</span></td>
	        </tr>
            <tr>
				<td class="row1" width="22%" align="right"><span class="genmed"><b>{L_POLL_QUESTION}</b></span></td>
				<td class="row2" width="78%"><span class="genmed"><input type="text" name="poll_title" size="50" maxlength="255" class="post" value="{POLL_TITLE}" /></span></td>
			</tr>
			<!-- BEGIN poll_option_rows -->
            <tr>
				<td class="row1" align="right"><span class="genmed"><b>{L_POLL_OPTION}</b></span></td>
				<td class="row2"><span class="genmed"><input type="text" name="poll_option_text[{poll_option_rows.S_POLL_OPTION_NUM}]" size="50" class="post" maxlength="255" value="{poll_option_rows.POLL_OPTION}" /></span> &nbsp;<input type="submit" name="edit_poll_option" value="{L_UPDATE_OPTION}" class="liteoption" /> <input type="submit" name="del_poll_option[{poll_option_rows.S_POLL_OPTION_NUM}]" value="{L_DELETE_OPTION}" class="liteoption" /></td>
			</tr>
			<!-- END poll_option_rows -->
            <tr>
				<td class="row1" align="right"><span class="genmed"><b>{L_POLL_OPTION}</b></span></td>
				<td class="row2"><span class="genmed"><input type="text" name="add_poll_option_text" size="50" maxlength="255" class="post" value="{ADD_POLL_OPTION}" /></span> &nbsp;<input type="submit" name="add_poll_option" value="{L_ADD_OPTION}" class="liteoption" /></td>
			</tr>
            <tr>
				<td class="row1" align="right"><span class="genmed"><b>{L_POLL_LENGTH}</b></span></td>
				<td class="row2"><span class="genmed"><input type="text" name="poll_length" size="3" maxlength="3" class="post" value="{POLL_LENGTH}" /></span>&nbsp;<span class="gen"><b>{L_DAYS}</b></span> &nbsp; <span class="gensmall">{L_POLL_LENGTH_EXPLAIN}</span></td>
			</tr>
			<!-- BEGIN switch_poll_delete_toggle -->
            <tr>
				<td class="row1" align="right"><span class="genmed"><b>{L_POLL_DELETE}</b></span></td>
				<td class="row2"><input type="checkbox" class="checkbox" name="poll_delete" /></td>
			</tr>
			<!-- END switch_poll_delete_toggle -->

