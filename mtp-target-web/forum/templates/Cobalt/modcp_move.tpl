
<form action="{S_MODCP_ACTION}" method="post">

  <table width="100%" cellspacing="2" cellpadding="2" border="0" align="center">
	<tr> 
	  <td align="left" class="nav"><a href="{U_INDEX}" class="nav">{L_INDEX}</a></td>
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
				  <th height="25" class="thHead"><b>{MESSAGE_TITLE}</b></th>
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
					  <td class="row1"> 
						<table width="100%" border="0" cellspacing="0" cellpadding="1">
						  <tr> 
							<td>&nbsp;</td>
						  </tr>
						  <tr> 
							<td align="center"><span class="gen">{L_MOVE_TO_FORUM} &nbsp; {S_FORUM_SELECT}<br /><br />
							  <input type="checkbox" class="checkbox" name="move_leave_shadow" checked />{L_LEAVESHADOW}<br />
							  <br /><span="messagetextwrap">
							  {MESSAGE_TEXT}</span></span><br />
							  <br />
							  {S_HIDDEN_FIELDS}
							  <input class="mainoption" type="submit" name="confirm" value="{L_YES}" />
							  &nbsp;&nbsp; 
							  <input class="liteoption" type="submit" name="cancel" value="{L_NO}" />
							</td>
						  </tr>
						  <tr> 
							<td>&nbsp;</td>
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

  

{S_HIDDEN_FIELDS}</form>
