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

					<table width="100%" cellpadding="4" cellspacing="1" border="0" class="forumline">
						<tr> 
						  <th class="thCornerL" height="25">{L_PM}</th>
						  <th class="thTop">{L_USERNAME}</th>
						  <th class="thTop">{L_POSTS}</th>
						  <th class="thTop">{L_FROM}</th>
						  <th class="thTop">{L_EMAIL}</th>
						  <th class="thTop">{L_WEBSITE}</th>
						  <th class="thCornerR">{L_SELECT}</th>
						</tr>
						<tr> 
						  <th class="thHead" colspan="8" height="28">{L_PENDING_MEMBERS}</th>
						</tr>
						<!-- BEGIN pending_members_row -->
						<tr> 
						  <td class="{pending_members_row.ROW_CLASS}" align="center"> {pending_members_row.PM_IMG} 
						  </td>
						  <td class="{pending_members_row.ROW_CLASS}" align="center"><span class="gen"><a href="{pending_members_row.U_VIEWPROFILE}" class="gen">{pending_members_row.USERNAME}</a></span></td>
						  <td class="{pending_members_row.ROW_CLASS}" align="center"><span class="gen">{pending_members_row.POSTS}</span></td>
						  <td class="{pending_members_row.ROW_CLASS}" align="center"><span class="gen">{pending_members_row.FROM}</span></td>
						  <td class="{pending_members_row.ROW_CLASS}" align="center"><span class="gen">{pending_members_row.EMAIL_IMG}</span></td>
						  <td class="{pending_members_row.ROW_CLASS}" align="center"><span class="gen">{pending_members_row.WWW_IMG}</span></td>
						  <td class="{pending_members_row.ROW_CLASS}" align="center"><span class="gensmall"> <input type="checkbox" class="checkbox" name="pending_members[]" value="{pending_members_row.USER_ID}" checked="checked" /></span></td>
						</tr>
						<!-- END pending_members_row -->
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
		<td width="100%" class="genBox" cellpadding="0">
		 		
					<table width="100%" cellspacing="0">
						<tr> 
						  <td class="catBottom" cellpadding="6" align="right">
							<input type="submit" name="approve" value="{L_APPROVE_SELECTED}" class="mainoption" />
							&nbsp; 
							<input type="submit" name="deny" value="{L_DENY_SELECTED}" class="liteoption" />&nbsp;
							</td>
						</tr>
					</table>
