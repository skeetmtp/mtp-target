<link rel="stylesheet" href="templates/Cobalt/Cobalt.css" type="text/css">
<link rel="stylesheet" href="/templates/Cobalt/Cobalt.css" type="text/css">
	<table width="100%" cellspacing="0" cellpadding="2" border="0" align="center">
		<tr>
			<td align="left" valign="bottom"><span class="gensmall">
			
			<!-- BEGIN switch_user_logged_in -->
	
				{LAST_VISIT_DATE}<BR/>
		
			<!-- END switch_user_logged_in -->
	
				{CURRENT_TIME}

			<BR/></span>

			<span class="nav"><A href="{U_INDEX}" class="nav">{L_INDEX}</a></span>
			
			</td>
			<td align="right" valign="bottom" class="gensmall">
			
			
			<!-- BEGIN switch_user_logged_in -->
			
			<A href="{U_SEARCH_NEW}" class="gensmall">{L_SEARCH_NEW}</a><BR/><A href="{U_SEARCH_SELF}" class="gensmall">{L_SEARCH_SELF}</a><BR/>
			
			<!-- END switch_user_logged_in -->
		
			<A href="{U_SEARCH_UNANSWERED}" class="gensmall">{L_SEARCH_UNANSWERED}</a>
		
			</td>
		</tr>
	</table>
	
	<BR>	
	
	<!-- BEGIN catrow -->

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
										<table border="0" cellspacing="0" cellpadding="0" width="100%">
											<tr>
												<td valign="top" width="0%"><img src="templates/Cobalt/images/cat_arrow.gif" width="25" height="39"></td>
												<td class="cattitle" width="100%"><span class="cattitle"><A href="{catrow.U_VIEWCAT}" class="cattitle">{catrow.CAT_DESC}</a></span></td>
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
			<td width="100%" valign="top">			
				
				
				<table width="100%" border="0" cellspacing="0" cellpadding="0">
					<tr>
						<td width="0%" class="cboxLeft"><img src="templates/Cobalt/images/spacer.gif" width="6" height="6"></td>
						<td width="100%" class="cbox" valign="top">
												
							<table width="100%" height="100%" border="0" cellspacing="0" cellpadding="0">
								<tr>
									<td class="cBoxStart">
									
										<!-- BEGIN forumrow -->
											<table width="100%" border="0" cellspacing="8" style="padding-left: 5px">
												<tr>
													<td width="0%" align="center"><DIV align="center"><img src="templates/Cobalt/images/spacer.gif" width="8" height="6"><img src="{catrow.forumrow.FORUM_FOLDER_IMG}" width="30" height="30" alt="{catrow.forumrow.L_FORUM_FOLDER_ALT}" title="{catrow.forumrow.L_FORUM_FOLDER_ALT}" class="img_forumstatus" /></DIV></td>
													<td width="80%"><span class="forumlink"><A href="{catrow.forumrow.U_VIEWFORUM}" class="forumlink">{catrow.forumrow.FORUM_NAME}</a><BR/></span><span class="forumdescription">{catrow.forumrow.FORUM_DESC}</span><br /><span class="forummoderator">{catrow.forumrow.L_MODERATOR} {catrow.forumrow.MODERATORS}</span></td>
													<td width="10%" nowrap="nowrap" align="center" class="forumstats"><strong><a href="#" class="forumstats" title="{L_POSTS}">{catrow.forumrow.POSTS}</a>&nbsp;/&nbsp;<a href="#" class="forumstats" title="{L_TOPICS}">{catrow.forumrow.TOPICS}</a></strong></td>
													<td width="160" align="center" nowrap><span class="gensmall">{catrow.forumrow.LAST_POST}</span></td>
												</tr>
											</table>											
										<!-- END forumrow -->
										
											<img src="templates/Cobalt/images/spacer.gif" width="6" height="6">
											
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
		<tr>
			<td colspan="3"><BR/></td>
		</tr>
	</table>	
	
	<!-- END catrow -->
	
	<table width="100%" cellspacing="0" border="0" align="center" cellpadding="2">
		<tr>
			<td align="left"><span class="gensmall"><A href="{U_MARK_READ}" class="gensmall">{L_MARK_FORUMS_READ}</a></span></td>
			<td align="right"><span class="gensmall">{S_TIMEZONE}</span></td>
		</tr>
	</table>
	
	<BR/><BR/>
	
	<table width="100%" border="0" cellspacing="0" cellpadding="0">
		<tr>
			<td colspan="3">
				
				<table width="100%" border="0" cellspacing="0" cellpadding="0">
					<tr>
						<td width="0%"><img src="templates/Cobalt/images/cat_lcap_whosonline.gif" width="22" height="51"></td>
						<td width="100%" background="templates/Cobalt/images/cat_bar.jpg" valign="top">
						
							<table width="100%" border="0" cellspacing="0" cellpadding="0" height="100%">
								<tr>
									<td class="cBarStart" valign="top">
									
										<table border="0" cellspacing="0" cellpadding="0" width="100%">
											<tr>
												<td valign="top" width="0%"><img src="templates/Cobalt/images/whosonline_item.gif" width="21" height="39"></td>
												<td class="cattitle" width="100%"><span class="cattitle"><A href="{U_VIEWONLINE}" class="cattitle">{L_WHO_IS_ONLINE}</a></span></td>
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
			<td width="0%"><img src="templates/Cobalt/images/spacer.gif" width="16" height="22" /></td>
			<td width="100%">
			
				<table width="100%" border="0" cellspacing="0" cellpadding="0">
					<tr>
						<td width="0%" class="cboxLeft"><img src="templates/Cobalt/images/spacer.gif" width="6" height="5"></td>
						<td width="100%" class="cbox">
							
							<table width="100%" border="0" cellspacing="0" cellpadding="0">
								<tr>
									<td class="cBoxStart">
									
										<table width="100%" border="0" cellspacing="8" cellpadding="0" style="padding-top: 1px">
											<tr>
												<td rowspan="2" align="center"><img src="templates/Cobalt/images/whosonline.gif" width="49" height="48" alt="{L_WHO_IS_ONLINE}"></td>
												<td><span class="genmed">{TOTAL_POSTS}<BR/>{TOTAL_USERS}<BR/>{NEWEST_USER}</span></td>
											</tr>
											<tr>
												<td class="onlineIndex"><span class="genmed">{TOTAL_USERS_ONLINE} &nbsp; [ {L_WHOSONLINE_ADMIN} ] &nbsp; [ {L_WHOSONLINE_MOD} ]<BR/>{RECORD_USERS}<BR/>{LOGGED_IN_USER_LIST}</span></td>
											</tr>
										</table>
										
									</td>
								</tr>
							</table>
							
						</td>
						<td width="0%" class="cboxRight"><img src="templates/Cobalt/images/spacer.gif" width="6" height="6">
						</td>
					</tr>
					<tr>
						<td width="0%" class="cboxLeftbottom"><img src="templates/Cobalt/images/spacer.gif" width="6" height="6"></td>
						<td width="100%" valign="top" class="cboxBottom"><img src="templates/Cobalt/images/spacer.gif" width="6" height="6"></td>
						<td width="0%" class="cboxRightbottom"><img src="templates/Cobalt/images/spacer.gif" width="6" height="6"></td>
					</tr>
				</table>
				
			</td>
			<td class="catbox_right"><img src="templates/Cobalt/images/spacer.gif" width="27" height="27">
			</td>
		</tr>
	</table>
	
	<table width="100%" cellpadding="1" cellspacing="1" border="0">
		<tr>
			<td align="left" valign="top"><span class="gensmall">{L_ONLINE_EXPLAIN}</span></td>
		</tr>
	</table>	
	
	<br /><br />
	
	<!-- BEGIN switch_user_logged_out -->
	
	<FORM method="post" action="{S_LOGIN_ACTION}">
	
		<table width="100%" border="0" cellspacing="0" cellpadding="0">
			<tr>
				<td colspan="3">
				
					<table width="100%" border="0" cellspacing="0" cellpadding="0">
						<tr>
							<td width="0%"><img src="templates/Cobalt/images/cat_lcap_whosonline.gif" width="22" height="51"></td>
							<td width="100%" background="templates/Cobalt/images/cat_bar.jpg" valign="top">
							
								<table width="100%" border="0" cellspacing="0" cellpadding="0" height="100%">
									<tr>
										<td class="cBarStart" valign="top">
										
											<table border="0" cellspacing="0" cellpadding="0">
												<tr>
													<td valign="top"><img src="templates/Cobalt/images/whosonline_item.gif" width="21" height="39"></td>
													<td class="cattitle"><span class="cattitle"><A name="login"></a>{L_LOGIN_LOGOUT}</span></td>
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
				<td width="0%"><img src="templates/Cobalt/images/spacer.gif" width="16" height="22">
				</td>
				<td width="100%">
					<table width="100%" border="0" cellspacing="0" cellpadding="0">
						<tr>
							<td width="0%" class="cboxLeft"><img src="templates/Cobalt/images/spacer.gif" width="6" height="5"></td>
							<td width="100%" class="cbox">
								<table width="100%" border="0" cellspacing="0" cellpadding="0">
									<tr>
										<td class="cBoxStart" align="center">
											<table border="0" cellspacing="8">
												<tr>
													<td class="loginIndex">
														<span class="genmed">
															{L_USERNAME}: <INPUT class="post" type="text" name="username" size="10" maxlength="32" />&nbsp;&nbsp;&nbsp;
															{L_PASSWORD}: <INPUT class="post" type="password" name="password" size="10" maxlength="32" />&nbsp;&nbsp;&nbsp;&nbsp;
															{L_AUTO_LOGIN}<INPUT type="checkbox" class="checkbox" name="autologin" />&nbsp;&nbsp;&nbsp;
															<INPUT type="submit" class="mainoption" name="login" value="{L_LOGIN}" />
														</span>
													</td>
												</tr>
											</table>
										</td>
									</tr>
								</table>
							</td>
							<td width="0%" class="cboxRight"><img src="templates/Cobalt/images/spacer.gif" width="6" height="6">
							</td>
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
		
		{S_HIDDEN_FIELDS}
		
	</FORM>	
	
	<!-- END switch_user_logged_out -->
	
	<BR clear="all"/>
	
	<table border="0" cellspacing="0" cellpadding="0" align="center">
		<tr>
			<td width="5" class="mainboxLefttop"><img src="templates/Cobalt/images/spacer.gif" width="6" height="6"></td>
			<td width="400" class="mainboxTop"><img src="templates/Cobalt/images/spacer.gif" width="6" height="6"></td>
			<td width="5" class="mainboxRighttop"><img src="templates/Cobalt/images/spacer.gif" width="6" height="6"></td>
		</tr>
		<tr>
			<td width="5" class="mainboxLeft"><img src="templates/Cobalt/images/spacer.gif" width="6" height="6">
			</td>
			<td width="400" class="folderIconBox">
				<table border="0" cellspacing="0" cellpadding="0" width="100%">
					<tr>
						<td class="folderIconBoxStart">
							<table cellspacing="0" border="0" align="center" cellpadding="5">
								<tr>
									<td width="23" align="center" style="padding-top: 5px; padding-bottom: 5px"><img src="templates/Cobalt/images/folder_new.gif" alt="{L_NEW_POSTS}" width="23" height="23" /></td>
									<td><span class="genmed">{L_NEW_POSTS}</span></td>
									<td>	&nbsp;&nbsp;	</td>
									<td width="23" align="center"><img src="templates/Cobalt/images/folder.gif" alt="{L_NO_NEW_POSTS}" width="23" height="23" /></td>
									<td><span class="genmed">{L_NO_NEW_POSTS}</span></td>
									<td>	&nbsp;&nbsp;	</td>
									<td width="23" align="center"><img src="templates/Cobalt/images/folder_lock.gif" alt="{L_FORUM_LOCKED}" width="23" height="23" /></td>
									<td><span class="genmed">{L_FORUM_LOCKED}</span></td>
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
