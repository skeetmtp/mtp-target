;!define MUI_PRODUCT "mtp-target" ;Define your own software name here
;!define MUI_VERSION "1.0.1" ;Define your own software version here
;!define MUI_OLD_VERSION "1.0.0"

; Script create for version 2.0b3 1.30 (from 09.mar.03) with GUI NSIS (c) by Dirk Paehl. Thank you for use my program
!include "MUI.nsh"
!include "mtp-target.nsh"

 
;--------------------------------
;Configuration
;version

 
 
;General
OutFile "${MUI_PRODUCT}-update-${MUI_OLD_VERSION}-${MUI_VERSION}.exe"

;Folder selection page
InstallDir "$PROGRAMFILES\${MUI_PRODUCT}"

;Remember install folder
InstallDirRegKey HKCU "Software\${MUI_PRODUCT}" ""



Function IsMtpInstalled
 Push $R0
 ClearErrors
 ReadRegStr $R0 HKCU "Software\${MUI_PRODUCT}" "version"
 IfErrors lbl_na
   StrCmp $R0 "${MUI_OLD_VERSION}" lbl_end
     MessageBox MB_OK|MB_ICONSTOP "This update if for version ${MUI_OLD_VERSION}" 
     Quit 
 lbl_na:
   MessageBox MB_OK|MB_ICONSTOP "Mtp Target not installed." 
   Quit 
 lbl_end:
 Pop $R0
FunctionEnd


;--------------------------------
;Modern UI Configuration
 !define MUI_DIRECTORYPAGE
 !define MUI_ABORTWARNING
 !define MUI_UNINSTALLER
 !define MUI_UNCONFIRMPAGE

 
;--------------------------------
 ;Language
 
  !insertmacro MUI_LANGUAGE "english"
;--------------------------------

;-------------------------------- 
;Installer Sections

Section "section_test_version" section_test_version
call IsMtpInstalled
SectionEnd

     
Section "section_1" section_1
SetOutPath "$INSTDIR"
FILE "client\ReleaseDebug\mtp-target.exe"
FILE "client\mtp_target_default.cfg"
FILE "server\ReleaseDebug\mtp_target_service.exe"
FILE "server\mtp_target_service_default.cfg"
SectionEnd
Section "section_2" section_2
SetOutPath "$INSTDIR\data"
FILE "client\data\lua\*.*"
SectionEnd

;Add Shortcuts
Section "toto"
CreateDirectory "$SMPROGRAMS\mtp-target"
SetOutPath $INSTDIR
CreateShortCut "$SMPROGRAMS\mtp-target\Play Mtp-Target.lnk" "$INSTDIR\mtp-target.exe" "" "$INSTDIR\mtp-target.exe" 0
CreateShortCut "$SMPROGRAMS\mtp-target\About.lnk" "http://mtptarget.free.fr/" "" "$INSTDIR\mtp-target.exe" 0
CreateShortCut "$SMPROGRAMS\mtp-target\Launch dedicated server.lnk" "$INSTDIR\mtp_target_service.exe" "" "$INSTDIR\mtp_target_service.exe" 0
CreateShortCut "$SMPROGRAMS\mtp-target\Uninstall Mtp-Target.lnk" "$INSTDIR\uninst.exe" "" "$INSTDIR\uninst.exe" 0
WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\mtp-target" "DisplayName" "mtp-target (remove only)"
WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\mtp-target" "UninstallString" "$INSTDIR\Uninst.exe"
WriteRegStr HKCU "Software\${MUI_PRODUCT}" "" $INSTDIR
WriteRegStr HKCU "Software\${MUI_PRODUCT}" "version" "${MUI_VERSION}"
WriteUninstaller "$INSTDIR\Uninst.exe"
 
;Display the Finish header
;Insert this macro after the sections if you are not using a finish page
 
SectionEnd
 
    !insertmacro MUI_SECTIONS_FINISHHEADER
 
;--------------------------------  
;Descriptions 
                                    
 
;--------------------------------
    
;Uninstaller Section
   
Section "Uninstall" 
 
  ;Add your stuff here  
   
  ;Delete Files 
 Delete "$INSTDIR\*.*" 
 Delete "$INSTDIR\cache\*.*" 
 RmDir "$INSTDIR\cache"
 Delete "$INSTDIR\data\*.*" 
 RmDir "$INSTDIR\data"
 Delete "$INSTDIR\replay\*.*" 
 RmDir "$INSTDIR\replay"
   
  ;Delete Start Menu Shortcuts
  Delete "$SMPROGRAMS\mtp-target\*.*"
  RmDir "$SMPROGRAMS\mtp-target"
  ;Delete Uninstaller And Unistall Registry Entries
  DeleteRegKey HKEY_LOCAL_MACHINE "SOFTWARE\mtp-target"
  DeleteRegKey HKEY_LOCAL_MACHINE "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\mtp-target"
  RMDir "$INSTDIR"
DeleteRegKey /ifempty HKCU "Software\${MUI_PRODUCT}"
  !insertmacro MUI_UNFINISHHEADER
             
SectionEnd
               
Function .onInstSuccess
   ExecShell open "$INSTDIR\config.exe"
FunctionEnd
   
;eof
