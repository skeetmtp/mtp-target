
<form method="post" action="{S_POST_DAYS_ACTION}">
  <table width="100%" cellspacing="2" cellpadding="2" border="0" align="center">
	<tr> 
	  <td align="left" valign="bottom" colspan="3"><a class="maintitle" href="{U_VIEW_FORUM}">{FORUM_NAME}</a><br /><span class="gensmall"><br />{L_MODERATOR}: {MODERATORS}<br />{LOGGED_IN_USER_LIST}</span></td>
	  <td align="right" valign="bottom" nowrap><span class="gensmall"><b>{PAGINATION}</b></span></td>
	</tr>
	<tr> 
	  <td align="left" valign="middle" width="50"><a href="{U_POST_NEW_TOPIC}"><img src="{POST_IMG}" border="0" alt="{L_POST_NEW_TOPIC}" /></a></td>
	  <td align="left" valign="middle" class="nav" width="100%" colspan="2"><span class="nav">&nbsp;&nbsp;&nbsp;<a href="{U_INDEX}" class="nav">{L_INDEX}</a> -> <a class="nav" href="{U_VIEW_FORUM}">{FORUM_NAME}</a></span></td>
	  <td align="right" valign="bottom" class="nav" nowrap><span class="gensmall"><a href="{U_MARK_READ}">{L_MARK_TOPICS_READ}</a></span></td>
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
      <td width="100%" class="folderIconBox"> 
	  
        <table border="0" cellspacing="0" cellpadding="0" width="100%">
          <tr> 
            <td class="folderIconBoxStart">

              <table border="0" cellpadding="4" cellspacing="1" width="100%" class="forumline">
                <tr>
                  <th colspan="2" align="center" height="25" class="thCornerL" nowrap>&nbsp;{L_TOPICS}&nbsp;</th>
                  <th width="50" align="center" class="thTop" nowrap>&nbsp;{L_REPLIES}&nbsp;</th>
                  <th width="100" align="center" class="thTop" nowrap>&nbsp;{L_AUTHOR}&nbsp;</th>
                  <th width="50" align="center" class="thTop" nowrap>&nbsp;{L_VIEWS}&nbsp;</th>
                  <th align="center" class="thCornerR" nowrap>&nbsp;{L_LASTPOST}&nbsp;</th>
                </tr>
                <!-- BEGIN topicrow -->
                <tr>
                  <td class="row1" align="center" valign="middle" width="20"><img src="{topicrow.TOPIC_FOLDER_IMG}" width="23" height="23" alt="{topicrow.L_TOPIC_FOLDER_ALT}" title="{topicrow.L_TOPIC_FOLDER_ALT}" class="img_topicstatus" /></td>
                  <td class="row1" width="100%"><span class="topictitle">{topicrow.NEWEST_POST_IMG}{topicrow.TOPIC_TYPE}<a href="{topicrow.U_VIEW_TOPIC}" class="topictitle">{topicrow.TOPIC_TITLE}</a></span><span class="gensmall"><br />
                    {topicrow.GOTO_PAGE}</span></td>
                  <td class="row2" align="center" valign="middle"><span class="postdetails">{topicrow.REPLIES}</span></td>
                  <td class="row3" align="center" valign="middle"><span class="name">{topicrow.TOPIC_AUTHOR}</span></td>
                  <td class="row2" align="center" valign="middle"><span class="postdetails">{topicrow.VIEWS}</span></td>
                  <td class="row3Right" align="center" valign="middle" nowrap><span class="postdetails">{topicrow.LAST_POST_TIME}<br />
                    {topicrow.LAST_POST_AUTHOR} {topicrow.LAST_POST_IMG}</span></td>
                </tr>
                <!-- END topicrow -->
                <!-- BEGIN switch_no_topics -->
                <tr> 
                  <td class="row1" colspan="6" height="30" align="center" valign="middle"><span class="gen">{L_NO_TOPICS}</span></td>
                </tr>
                <!-- END switch_no_topics -->
                <tr> 
                  <td class="catBottom" align="center" valign="middle" colspan="6" height="28"><span class="genmed"><span class="bottom">{L_DISPLAY_TOPICS}:&nbsp;{S_SELECT_TOPIC_DAYS}&nbsp;
                    <input type="submit" class="liteoption" value="{L_GO}" name="submit" />
                    </span></span></td>
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
	  <td align="left" valign="middle" width="50" class="tdPostImgBottom"><a href="{U_POST_NEW_TOPIC}"><img src="{POST_IMG}" border="0" alt="{L_POST_NEW_TOPIC}" /></a></td>
	  <td align="left" valign="middle" width="100%"><span class="nav">&nbsp;&nbsp;&nbsp;<a href="{U_INDEX}" class="nav">{L_INDEX}</a> -> <a class="nav" href="{U_VIEW_FORUM}">{FORUM_NAME}</a></span></td>
	  <td align="right" valign="middle" nowrap><span class="gensmall">{S_TIMEZONE}</span><br /><span class="nav">{PAGINATION}</span> 
		</td>
	</tr>
	<tr>
	  <td align="left" colspan="3"><span class="nav">{PAGE_NUMBER}</span></td>
	</tr>
  </table>
{S_HIDDEN_FIELDS}</form>

<div align="right">{JUMPBOX}</div>

<table width="100%" cellspacing="0" border="0" align="center" cellpadding="0">
	<tr>
		<td align="left" valign="top">

			<table border="0" cellspacing="0" cellpadding="0">
				<tr> 
				  <td width="5" class="mainboxLefttop"><img src="templates/Cobalt/images/spacer.gif" width="6" height="6"></td>
				  <td class="mainboxTop"><img src="templates/Cobalt/images/spacer.gif" width="6" height="6"></td>
				  <td width="5" class="mainboxRighttop"><img src="templates/Cobalt/images/spacer.gif" width="6" height="6"></td>
				</tr>
				<tr>
				  <td width="5" class="mainboxLeft"><img src="templates/Cobalt/images/spacer.gif" width="6" height="6"></td>
				  <td class="folderIconBox">

					<table border="0" cellspacing="0" cellpadding="0">
					  <tr>
						<td class="folderIconBoxStart"> 
							
                                          <table cellspacing="3" cellpadding="0" border="0">
                                            <tr>
                                              <td width="23" align="left" nowrap><img src="{FOLDER_NEW_IMG}" alt="{L_NEW_POSTS}" width="23" height="23" /></td>
                                              <td class="genmed" nowrap>{L_NEW_POSTS}</td>
                                              <td>&nbsp;&nbsp;</td>
                                              <td width="23" align="center" nowrap><img src="{FOLDER_IMG}" alt="{L_NO_NEW_POSTS}" width="23" height="23" /></td>
                                              <td class="genmed" nowrap>{L_NO_NEW_POSTS}</td>
                                              <td>&nbsp;&nbsp;</td>
                                              <td width="23" align="center" nowrap><img src="{FOLDER_ANNOUNCE_IMG}" alt="{L_ANNOUNCEMENT}" width="23" height="23" /></td>
                                              <td class="genmed" nowrap>{L_ANNOUNCEMENT}</td>
                                            </tr>
                                            <tr> 
                                              <td width="23" align="center" nowrap><img src="{FOLDER_LOCKED_NEW_IMG}" alt="{L_NEW_POSTS_TOPIC_LOCKED}" width="23" height="23" /></td>
                                              <td class="genmed" nowrap>{L_NEW_POSTS_LOCKED}</td>
                                              <td>&nbsp;&nbsp;</td>
                                              <td width="23" align="center" nowrap><img src="{FOLDER_LOCKED_IMG}" alt="{L_NO_NEW_POSTS_TOPIC_LOCKED}" width="23" height="23" /></td>
                                              <td class="genmed" nowrap>{L_NO_NEW_POSTS_LOCKED}</td>
                                              <td>&nbsp;&nbsp;</td>
                                              <td width="23" align="center" nowrap><img src="{FOLDER_STICKY_IMG}" alt="{L_STICKY}" width="23" height="23" /></td>
                                              <td class="genmed" nowrap>{L_STICKY}</td>
                                            </tr>
                                          </table>
						</td>

					  </tr>
					</table>

				  </td>
				  <td width="5" class="mainboxRight"><img src="templates/Cobalt/images/spacer.gif" width="6" height="6"></td>
				</tr>
				<tr> 
				  <td width="5" class="mainboxLeftbottom"><img src="templates/Cobalt/images/spacer.gif" width="6" height="6"></td>
				  <td width="400" valign="top" class="mainboxBottom"><img src="templates/Cobalt/images/spacer.gif" width="6" height="6"></td>
				  <td width="5" class="mainboxRightbottom"><img src="templates/Cobalt/images/spacer.gif" width="6" height="6"></td>
				</tr>
			  </table>
			</td>
		<td align="right"><span class="gensmall">{S_AUTH_LIST}</span></td>
	</tr>
</table>
