
<form method="post" action="{S_SPLIT_ACTION}">
  <table width="100%" cellspacing="2" cellpadding="2" border="0" align="center">
	<tr> 
	  <td align="left" class="nav"><a href="{U_INDEX}" class="nav">{L_INDEX}</a><span class="nav"> 
		-> <a href="{U_VIEW_FORUM}" class="nav">{FORUM_NAME}</a></span></td>
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
					  <th height="25" class="thHead" colspan="3" nowrap>{L_SPLIT_TOPIC}</th>
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
			
			
			  <table width="100%" cellpadding="4" cellspacing="0" border="0">
				<tr> 
				  <td class="row2" colspan="3" align="center"><span class="gensmall">{L_SPLIT_TOPIC_EXPLAIN}</span></td>
				</tr>
				<tr> 
				  <td class="row1" nowrap><span class="gen">{L_SPLIT_SUBJECT}</span></td>
				  <td class="row2" colspan="2"><span class="courier"> 
					<input type="text" size="35" style="width: 350px" maxlength="100" name="subject" class="post" />
					</span></td>
				</tr>
				<tr> 
				  <td class="row1" nowrap><span class="gen">{L_SPLIT_FORUM}</span></td>
				  <td class="row2" colspan="2"><span class="courier">{S_FORUM_SELECT}</span></td>
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
			
			
			
			<table width="100%" cellpadding="4" cellspacing="0" border="0">
				<tr> 
				  <td class="catHead" colspan="3" height="28"> 
					
					<table width="60%" cellspacing="0" cellpadding="0" border="0" align="center">
					  <tr> 
						<td width="50%" align="center"> 
						  <input class="liteoption" type="submit" name="split_type_all" value="{L_SPLIT_POSTS}" />
						</td>
						<td width="50%" align="center"> 
						  <input class="liteoption" type="submit" name="split_type_beyond" value="{L_SPLIT_AFTER}" />
						</td>
					  </tr>
					</table>
					
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
      <td width="100%" class="ErrorConfirmBox"> 
        <table border="0" cellspacing="0" cellpadding="0" width="100%">
          <tr> 
            <td class="ErrorConfirmBoxStart">				  
			  
				
			  <table width="100%" cellpadding="4" cellspacing="1" border="0"> 				
				<tr> 
				  <th class="thLeft" nowrap>{L_AUTHOR}</th>
				  <th nowrap>{L_MESSAGE}</th>
				  <th class="thRight" nowrap>{L_SELECT}</th>
				</tr>
				<!-- BEGIN postrow -->
				<tr> 
				  <td align="left" valign="top" class="{postrow.ROW_CLASS}"><span class="name"><a name="{postrow.U_POST_ID}"></a>{postrow.POSTER_NAME}</span></td>
				  <td width="100%" valign="top" class="{postrow.ROW_CLASS}"> 
					<table width="100%" cellspacing="0" cellpadding="3" border="0">
					  <tr> 
						<td valign="middle"><img src="templates/Cobalt/images/minipost_read.gif" alt="{L_POST}"><span class="postdetails">{L_POSTED}:
						  {postrow.POST_DATE}&nbsp;&nbsp;&nbsp;&nbsp;{L_POST_SUBJECT}: {postrow.POST_SUBJECT}</span></td>
					  </tr>
					  <tr> 
						<td valign="top"> 
						  <hr size="1" />
						  <span class="postbody">{postrow.MESSAGE}</span></td> 
					  </tr>
					</table>
				  </td>
				  <td width="5%" align="center" class="{postrow.ROW_CLASS}">{postrow.S_SPLIT_CHECKBOX}</td>
				</tr>
				<tr> 
				  <td colspan="3" height="1" class="row3"><img src="templates/Cobalt/images/spacer.gif" width="1" height="1" alt="."></td>
				</tr>
				<!-- END postrow -->
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
			
			<table width="100%" cellpadding="4" cellspacing="0" border="0"> 			
				<tr> 
				  <td class="catBottom" colspan="3" height="28"> 
					<table width="60%" cellspacing="0" cellpadding="0" border="0" align="center">
					  <tr> 
						<td width="50%" align="center"> 
						  <input class="liteoption" type="submit" name="split_type_all" value="{L_SPLIT_POSTS}" />
						</td>
						<td width="50%" align="center"> 
						  <input class="liteoption" type="submit" name="split_type_beyond" value="{L_SPLIT_AFTER}" />
						  {S_HIDDEN_FIELDS} </td>
					  </tr>
					</table>
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
  <p>&nbsp;</p>
  <p>&nbsp;</p>

  
  <table width="100%" cellspacing="2" border="0" align="center" cellpadding="2">
	<tr> 
	  <td align="right" valign="top"><span class="gensmall">{S_TIMEZONE}</span></td>
	</tr>
  </table>
{S_HIDDEN_FIELDS}</form>
