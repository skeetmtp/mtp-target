
<form method="post" action="{S_MODCP_ACTION}">
<table width="100%" cellspacing="2" cellpadding="2" border="0" align="center">
  <tr> 
	<td align="left"><span class="nav"><a href="{U_INDEX}" class="nav">{L_INDEX}</a> -> <a href="{U_VIEW_FORUM}" class="nav">{FORUM_NAME}</a></span></td>
  </tr>
</table>

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
			
				  <table width="100%" cellpadding="4" cellspacing="0" border="0">
					<tr> 
					  <th class="thHead" colspan="5" align="center" height="28">{L_MOD_CP}</th>
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
			
				  <table width="100%" cellpadding="4" cellspacing="1" border="0">
					<tr> 
					  <td class="spaceRow" colspan="5" align="center"><span class="gensmall">{L_MOD_CP_EXPLAIN}</span></td>
					</tr>
					<tr> 
					  <th width="4%" class="thLeft" nowrap>&nbsp;</th>
					  <th nowrap>&nbsp;{L_TOPICS}&nbsp;</th>
					  <th width="8%" nowrap>&nbsp;{L_REPLIES}&nbsp;</th>
					  <th width="17%" nowrap>&nbsp;{L_LASTPOST}&nbsp;</th>
					  <th width="5%" class="thRight" nowrap>&nbsp;{L_SELECT}&nbsp;</th>
					</tr>
					<!-- BEGIN topicrow -->
					<tr> 
					  <td class="row1" align="center" valign="middle"><img src="{topicrow.TOPIC_FOLDER_IMG}" width="23" height="23" alt="{topicrow.L_TOPIC_FOLDER_ALT}" title="{topicrow.L_TOPIC_FOLDER_ALT}" /></td>
					  <td class="row1">&nbsp;<span class="topictitle">{topicrow.TOPIC_TYPE}<a href="{topicrow.U_VIEW_TOPIC}" class="topictitle">{topicrow.TOPIC_TITLE}</a></span></td>
					  <td class="row2" align="center" valign="middle"><span class="postdetails">{topicrow.REPLIES}</span></td>
					  <td class="row1" align="center" valign="middle"><span class="postdetails">{topicrow.LAST_POST_TIME}</span></td>
					  <td class="row2" align="center" valign="middle"> 
						<input type="checkbox" class="checkbox" name="topic_id_list[]" value="{topicrow.TOPIC_ID}" />
					  </td>
					</tr>
					<!-- END topicrow -->
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
				
				
				<table width="100%" cellpadding="4" cellspacing="1" border="0">
					<tr align="right"> 
					  <td class="catBottom" colspan="5" height="29"> {S_HIDDEN_FIELDS} 
						<input type="submit" name="delete" class="liteoption" value="{L_DELETE}" />
						&nbsp; 
						<input type="submit" name="move" class="liteoption" value="{L_MOVE}" />
						&nbsp; 
						<input type="submit" name="lock" class="liteoption" value="{L_LOCK}" />
						&nbsp; 
						<input type="submit" name="unlock" class="liteoption" value="{L_UNLOCK}" />
					  </td>
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
  
  <table width="100%" cellspacing="2" border="0" align="center" cellpadding="2">
  <tr> 
	<td align="left" valign="middle"><span class="nav">{PAGE_NUMBER}</span></td>
	<td align="right" valign="top" nowrap><span class="gensmall">{S_TIMEZONE}</span><br /><span class="nav">{PAGINATION}</span></td>
  </tr>
</table>
{S_HIDDEN_FIELDS}</form>
<table width="100%" border="0" cellspacing="0" cellpadding="0">
  <tr> 
	<td align="right">{JUMPBOX}</td>
  </tr>
</table>
