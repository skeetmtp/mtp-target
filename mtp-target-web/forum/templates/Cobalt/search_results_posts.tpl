 
<table width="100%" cellspacing="2" cellpadding="2" border="0" align="center">
  <tr> 
	<td align="left"><span class="nav"><a href="{U_INDEX}" class="nav">{L_INDEX}</a></span></td>
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
    <td width="100%" class="genBox"> 
      <table border="0" cellspacing="0" cellpadding="0" width="100%">
        <tr> 
          <td class="genBoxStart">
            <table width="100%" border="0">
              <tr>
                <th class="thHead">{L_SEARCH_MATCHES}</th>
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
            <table border="0" cellpadding="3" cellspacing="1" width="100%" class="forumline" align="center">
              <tr> 
                <th width="150" height="25" class="thCornerL" nowrap>{L_AUTHOR}</th>
                <th width="100%" class="thCornerR" nowrap>{L_MESSAGE}</th>
              </tr>
              <!-- BEGIN searchresults -->
              <tr> 
                <td class="catHead" colspan="2" height="28"><span class="topictitle"><img src="templates/Cobalt/images/folder.gif" align="absmiddle">&nbsp; 
                  {L_TOPIC}:&nbsp;<a href="{searchresults.U_TOPIC}" class="topictitle">{searchresults.TOPIC_TITLE}</a></span></td>
              </tr>
              <tr> 
                <td width="150" align="left" valign="top" class="row1" rowspan="2"><span class="name"><b>{searchresults.POSTER_NAME}</b></span><br />
                  <br />
                  <span class="postdetails">{L_REPLIES}: <b>{searchresults.TOPIC_REPLIES}</b><br />
                  {L_VIEWS}: <b>{searchresults.TOPIC_VIEWS}</b></span><br />
                </td>
                <td width="100%" valign="top" class="row1"><img src="{searchresults.MINI_POST_IMG}" width="12" height="9" alt="{searchresults.L_MINI_POST_ALT}" title="{searchresults.L_MINI_POST_ALT}" border="0" /><span class="postdetails">{L_FORUM}:&nbsp;<b><a href="{searchresults.U_FORUM}" class="postdetails">{searchresults.FORUM_NAME}</a></b>&nbsp; 
                  &nbsp;{L_POSTED}: {searchresults.POST_DATE}&nbsp; &nbsp;{L_SUBJECT}: 
                  <b><a href="{searchresults.U_POST}">{searchresults.POST_SUBJECT}</a></b></span></td>
              </tr>
              <tr> 
                <td valign="top" class="row1"><span class="postbody">{searchresults.MESSAGE}</span></td>
              </tr>
              <!-- END searchresults -->
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
	<td align="left" valign="top"><span class="nav">{PAGE_NUMBER}</span></td>
	<td align="right" valign="top" nowrap><span class="nav">{PAGINATION}</span><br /><span class="gensmall">{S_TIMEZONE}</span></td>
  </tr>
</table>

<table width="100%" cellspacing="2" border="0" align="center">
  <tr> 
	<td valign="top" align="right">{JUMPBOX}</td>
  </tr>
</table>
