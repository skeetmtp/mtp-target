
<form action="{S_SEARCH_ACTION}" method="POST"><table width="100%" cellspacing="2" cellpadding="2" border="0" align="center">
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
                        <td class="cattitle">{L_SEARCH_QUERY}</td>
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
				  
                    <table width="100%" cellpadding="4" cellspacing="1" border="0">
                      <tr> 
                        <td class="row1" width="50%"><span class="gen">{L_SEARCH_KEYWORDS}:</span><br />
                          <span class="gensmall">{L_SEARCH_KEYWORDS_EXPLAIN}</span></td>
                        <td class="row2" valign="top" width="50%"><span class="genmed"> 
                          <input type="text" style="width: 300px" class="post" name="search_keywords" size="30" />
                          <br />
                          <input type="radio" class="checkbox" name="search_terms" value="any" checked />
                          {L_SEARCH_ANY_TERMS}<br />
                          <input type="radio" class="checkbox" name="search_terms" value="all" />
                          {L_SEARCH_ALL_TERMS}</span></td>
                      </tr>
                      <tr>
                        <td class="row1" width="50%">&nbsp;</td>
                        <td class="row2" width="50%">&nbsp;</td>
                      </tr>
                      <tr>
                        <td class="row1" width="50%"><span class="gen">{L_SEARCH_AUTHOR}:</span><br />
                          <span class="gensmall">{L_SEARCH_AUTHOR_EXPLAIN}</span></td>
                        <td class="row2" width="50%"><span class="genmed"> 
                          <input type="text" style="width: 300px" class="post" name="search_author" size="30" />
                          </span></td>
                      </tr>
                      <tr>
                        <td class="row1" width="50%">&nbsp;</td>
                        <td class="row2" width="50%">&nbsp;</td>
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
		
              <table width="100%" border="0" cellspacing="0" cellpadding="0">
                <tr> 
                  <td class="genBoxStart">		 			  					
					
                    <table class="forumline" width="100%" cellpadding="4" cellspacing="0" border="0">
                      <tr> 
                        <th class="thHead" colspan="4" height="25">{L_SEARCH_OPTIONS}</th>
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
		
              <table width="100%" border="0" cellspacing="0" cellpadding="0">
                <tr>
                  <td class="genBoxStart" style="padding-top: 15px">

                    <table class="forumline" width="100%" cellpadding="4" cellspacing="1" border="0">
                      <tr> 
                        <td class="row1" align="right"><span class="gen">{L_FORUM}:&nbsp;</span></td>
                        <td class="row2"><span class="genmed">
                          <select class="post" name="search_forum">{S_FORUM_OPTIONS}
                          </select>
                          </span></td>
                        <td class="row1" align="right" nowrap valign="top"><span class="gen">{L_SEARCH_PREVIOUS}:&nbsp;</span></td>
                        <td class="row2" valign="middle"><span class="genmed">
                          <select class="post" name="search_time">{S_TIME_OPTIONS}
                          </select>
                          <br />
                          <input type="radio" class="checkbox" name="search_fields" value="all" checked />
                          {L_SEARCH_MESSAGE_TITLE}<br />
                          <input type="radio" class="checkbox" name="search_fields" value="msgonly" />
                          {L_SEARCH_MESSAGE_ONLY}</span></td>
                      </tr>
                      <tr> 
                        <td class="row1" align="right"><span class="gen">{L_CATEGORY}:&nbsp;</span></td>
                        <td class="row2"><span class="genmed">
                          <select class="post" name="search_cat">{S_CATEGORY_OPTIONS}
		                  </select>
                          </span></td>
                        <td class="row1" align="right" valign="top"><span class="gen">{L_SORT_BY}:&nbsp;</span></td>
                        <td class="row2" valign="middle" nowrap><span class="genmed">
                          <select class="post" name="sort_by">{S_SORT_OPTIONS}
                          </select>
                          <br />
                          <input type="radio" class="checkbox" name="sort_dir" value="ASC" />
                          {L_SORT_ASCENDING}<br />
                          <input type="radio" class="checkbox" name="sort_dir" value="DESC" checked />
                          {L_SORT_DESCENDING}</span>&nbsp;</td>
                      </tr>
                      <tr> 
                        <td class="row1" align="right" nowrap><span class="gen">{L_DISPLAY_RESULTS}:&nbsp;</span></td>
                        <td class="row2" nowrap>
                          <input type="radio" class="checkbox" name="show_results" value="posts" />
                          <span class="genmed">{L_POSTS}
                          <input type="radio" class="checkbox" name="show_results" value="topics" checked />
                          {L_TOPICS}</span></td>
                        <td class="row1" align="right" valign="top"><span class="gen">{L_RETURN_FIRST}</span></td>
                        <td class="row2" style="padding-bottom: 15px"><span class="genmed">
                          <select class="post" name="return_chars">{S_CHARACTER_OPTIONS}
                          </select>
                          {L_CHARACTERS}</span>
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

                   <table cellspacing="0" width="100%">
                      <tr>
                        <td class="catBottom" align="center" height="28"><input class="liteoption" type="submit" value="{L_SEARCH}" name="submit" /></td>
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

  <table width="100%" cellspacing="2" cellpadding="2" border="0" align="center">
	<tr> 
		<td align="right" valign="middle"><span class="gensmall">{S_TIMEZONE}</span></td>
	</tr>
</table>
{S_HIDDEN_FIELDS}
</form>

<table width="100%" border="0">
	<tr>
		<td align="right" valign="top">{JUMPBOX}</td>
	</tr>
</table>
