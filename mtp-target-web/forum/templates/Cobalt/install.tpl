<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1">
<meta http-equiv="Content-Style-Type" content="text/css">
<title>{L_INSTALLATION}</title>
<link rel="stylesheet" href="templates/Cobalt/Cobalt.css" type="text/css">
<style type="text/css">
<!--
/* Specifiy background images for selected styles
   This can't be done within the external style sheet as NS4 sees image paths relative to
   the page which called the style sheet (i.e. this page in the root phpBB directory)
   whereas all other browsers see image paths relative to the style sheet. Stupid NS again!
*/
TH			{ background-image: url(templates/Cobalt/images/cellpic3.gif) }
TD.cat		{ background-image: url(templates/Cobalt/images/cellpic1.gif) }
TD.rowpic	{ background-image: url(templates/Cobalt/images/cellpic2.jpg); background-repeat: repeat-y }
td.icqback	{ background-image: url(templates/Cobalt/images/icon_icq_add.gif); background-repeat: no-repeat }
TD.catHead,TD.catSides,TD.catLeft,TD.catRight,TD.catBottom { background-image: url(templates/Cobalt/images/cellpic1.gif) }

/* Import the fancy styles for IE only (NS4.x doesn't use the @import function) */
@import url("templates/Cobalt/formIE.css"); 
-->
</style>
</head>
<body bgcolor="#E5E5E5" text="#000000" link="#006699" vlink="#5584AA">

<table width="100%" border="0" cellspacing="0" cellpadding="10" align="center"> 
	<tr>
		
    <td class="bodyline" width="100%"> 
      <p><img src="templates/Cobalt/images/logo_phpBB.gif" border="0" alt="Forum Home" vspace="1" /> 
        <br>
      </p>
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
                            <td class="cattitle"><span class="cattitle">{L_INSTALLATION}</span></td>
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
                        <table width="100%" cellpadding="2" cellspacing="1" border="0">
                          <tr> 
                            <td colspan="2" align="center"> 
                              <table width="80%" border="0">
                                <tr> 
                                  <td><span class="gen">{L_INSTRUCTION_TEXT}</span> 
                                  </td>
                                </tr>
                              </table>
                            </td>
                          </tr>
                          <!-- BEGIN switch_stage_one_install -->
                          <tr> 
                            <th colspan="2">{L_INITIAL_CONFIGURATION}</th>
                          </tr>
                          <tr> 
                            <td class="row1" align="right" width="30%"><span class="gen">{L_LANGUAGE}: 
                              </span></td>
                            <td class="row2">{S_LANG_SELECT}</td>
                          </tr>
                          <tr> 
                            <td class="row1" align="right"><span class="gen">{L_DBMS}: 
                              </span></td>
                            <td class="row2">{S_DBMS_SELECT}</td>
                          </tr>
                          <tr> 
                            <td class="row1" align="right"><span class="gen">{L_UPGRADE}:</span></td>
                            <td class="row2">{S_UPGRADE_SELECT}</td>
                          </tr>
                          <tr> 
                            <th colspan="2">{L_DATABASE_CONFIGURATION}</th>
                          </tr>
                          <tr> 
                            <td class="row1" align="right"><span class="gen">{L_DB_HOST}: 
                              </span></td>
                            <td class="row2"> 
                              <input type="text" name="dbhost" value="{DB_HOST}" />
                            </td>
                          </tr>
                          <tr> 
                            <td class="row1" align="right"><span class="gen">{L_DB_NAME}: 
                              </span></td>
                            <td class="row2"> 
                              <input type="text" name="dbname" value="{DB_NAME}" />
                            </td>
                          </tr>
                          <tr> 
                            <td class="row1" align="right"><span class="gen">{L_DB_USER}: 
                              </span></td>
                            <td class="row2"> 
                              <input type="text" name="dbuser" value="{DB_USER}" />
                            </td>
                          </tr>
                          <tr> 
                            <td class="row1" align="right"><span class="gen">{L_DB_PASSWORD}: 
                              </span></td>
                            <td class="row2"> 
                              <input type="password" name="dbpasswd" />
                            </td>
                          </tr>
                          <tr> 
                            <td class="row1" align="right"><span class="gen">{L_DB_PREFIX}: 
                              </span></td>
                            <td class="row2"> 
                              <input type="text" name="prefix" value="{DB_PREFIX}" />
                            </td>
                          </tr>
                          <tr> 
                            <th colspan="2">{L_ADMIN_CONFIGURATION}</th>
                          </tr>
                          <tr> 
                            <td class="row1" align="right"><span class="gen">{L_ADMIN_EMAIL}: 
                              </span></td>
                            <td class="row2"> 
                              <input type="text" name="board_email" />
                            </td>
                          </tr>
                          <tr> 
                            <td class="row1" align="right"><span class="gen">{L_SERVER_NAME}: 
                              </span></td>
                            <td class="row2"> 
                              <input type="text" name="server_name" value="{SERVER_NAME}" />
                            </td>
                          </tr>
                          <tr> 
                            <td class="row1" align="right"><span class="gen">{L_SERVER_PORT}: 
                              </span></td>
                            <td class="row2"> 
                              <input type="text" name="server_port" value="{SERVER_PORT}" />
                            </td>
                          </tr>
                          <tr> 
                            <td class="row1" align="right"><span class="gen">{L_SCRIPT_PATH}: 
                              </span></td>
                            <td class="row2"> 
                              <input type="text" name="script_path" value="{SCRIPT_PATH}" />
                            </td>
                          </tr>
                          <tr> 
                            <td class="row1" align="right"><span class="gen">{L_ADMIN_USERNAME}: 
                              </span></td>
                            <td class="row2"> 
                              <input type="text" name="admin_name" value="{ADMIN_USERNAME}" />
                            </td>
                          </tr>
                          <tr> 
                            <td class="row1" align="right"><span class="gen">{L_ADMIN_PASSWORD}: 
                              </span></td>
                            <td class="row2"> 
                              <input type="password" name="admin_pass1" />
                            </td>
                          </tr>
                          <tr> 
                            <td class="row1" align="right"><span class="gen">{L_ADMIN_CONFIRM_PASSWORD}: 
                              </span></td>
                            <td class="row2"> 
                              <input type="password" name="admin_pass2" />
                            </td>
                          </tr>
                          <!-- END switch_stage_one_install -->
                          <!-- BEGIN switch_error_install -->
                          <tr> 
                            <th>{L_ERROR_TITLE}</th>
                          </tr>
                          <tr> 
                            <td class="row1" align="center"><span class="gen">{L_ERROR}</span></td>
                          </tr>
                          <!-- END switch_error_install -->
                          <!-- BEGIN switch_ftp_file -->
                          <tr> 
                            <th colspan="2">{L_FTP_INFO}</th>
                          </tr>
                          <tr> 
                            <td class="row1" align="right"><span class="gen">{L_FTP_PATH}</span></td>
                            <td class="row2"> 
                              <input type="text" name="ftp_dir">
                            </td>
                          </tr>
                          <tr> 
                            <td class="row1" align="right"><span class="gen">{L_FTP_USER}</span></td>
                            <td class="row2"> 
                              <input type="text" name="ftp_user">
                            </td>
                          </tr>
                          <tr> 
                            <td class="row1" align="right"><span class="gen">{L_FTP_PASS}</span></td>
                            <td class="row2"> 
                              <input type="password" name="ftp_pass">
                            </td>
                          </tr>
                          <!-- END switch_ftp_file -->
                          <!-- BEGIN switch_ftp_option -->
                          <tr> 
                            <th colspan="2">{L_CHOOSE_FTP}</th>
                          </tr>
                          <tr> 
                            <td class="row1" align="right" width="50%"><span class="gen">{L_ATTEMPT_FTP}</span></td>
                            <td class="row2"> 
                              <input type="radio" class="checkbox" name="send_file" value="2">
                            </td>
                          </tr>
                          <tr> 
                            <td class="row1" align="right" width="50%"><span class="gen">{L_SEND_FILE}</span></td>
                            <td class="row2"> 
                              <input type="radio" class="checkbox" name="send_file" value="1">
                            </td>
                          </tr>
                          <!-- END switch_ftp_option -->
                          <!-- BEGIN switch_common_install -->
                          <tr> 
                            <td class="catbottom" align="center" colspan="2">{S_HIDDEN_FIELDS} 
                              <input class="mainoption" type="submit" value="{L_SUBMIT}" name="submit" />
                            </td>
                          </tr>
                          <!-- END switch_common_install -->
                          <!-- BEGIN switch_upgrade_install -->
                          <tr> 
                            <td class="catbottom" align="center" colspan="2">{L_UPGRADE_INST}</td>
                          </tr>
                          <tr> 
                            <td class="catbottom" align="center" colspan="2"> 
                              <input type="submit" name="upgrade_now" value="{L_UPGRADE_SUBMIT}" />
                            </td>
                          </tr>
                          <!-- END switch_upgrade_install -->
                        </table>
                      </td>
                    </tr>
                  </table>
                </td>
                <td width="0%" class="cboxRight"><img src="templates/Cobalt/images/spacer.gif" width="6" height="6"></td>
              </tr>
              <tr> 
                <td width="0%" class="cboxLeftbottom">&nbsp;</td>
                <td width="100%" valign="top" class="cboxBottom"><img src="templates/Cobalt/images/spacer.gif" width="6" height="6"></td>
                <td width="0%" class="cboxRightbottom">&nbsp;</td>
              </tr>
            </table>
          </td>
          <td class="catbox_right"><img src="templates/Cobalt/images/spacer.gif" width="27" height="27"></td>
        </tr>
      </table>
    
    </td>
	</tr>
</table>

</body>
</html>
