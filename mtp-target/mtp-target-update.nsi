; Modern interface settings
!include "MUI.nsh"
!include "mtp-target.nsh"

; Main Install settings
Name "${APPNAMEANDVERSION}"
InstallDir "$PROGRAMFILES\${APPNAME}"
InstallDirRegKey HKLM "Software\${APPNAME}" ""
OutFile "${APPNAME}-update-${APPOLDVERSION}-${APPVERSION}.exe"


!define MUI_ABORTWARNING

!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_COMPONENTS
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES

; Set languages (first is default language)
!insertmacro MUI_LANGUAGE "English"
!insertmacro MUI_RESERVEFILE_LANGDLL

Section "base" Section1
	call IsMtptInstalled
	; Set Section properties
	SectionIn RO
	SetOverwrite on
	
	SetOutPath "$INSTDIR"
	FILE "client\ReleaseDebug\mtp-target.exe"
	FILE "client\mtp_target_default.cfg"
	FILE "server\ReleaseDebug\mtp_target_service.exe"
	FILE "server\mtp_target_service_default.cfg"
	; Set Section Files and Shortcuts
	CreateDirectory "$SMPROGRAMS\${APPNAME}"
	CreateShortCut "$SMPROGRAMS\${APPNAME}\Play Mtp-Target.lnk" "$INSTDIR\mtp-target.exe"
	CreateShortCut "$SMPROGRAMS\${APPNAME}\About.lnk" "http://mtptarget.free.fr/" "" "$INSTDIR\mtp-target.exe" 0
	
	;WriteUninstaller "$INSTDIR\Uninst.exe"
	CreateShortCut "$SMPROGRAMS\${APPNAME}\Uninstall.lnk" "$INSTDIR\uninstall.exe"

SectionEnd

Section "server" Section2
	call IsMtptInstalled
	; Set Section properties
	SetOverwrite on
	
	SetOutPath "$INSTDIR"
	FILE "server\ReleaseDebug\mtp_target_service.exe"
	FILE "server\mtp_target_service_default.cfg"

	; Set Section Files and Shortcuts
	CreateDirectory "$SMPROGRAMS\${APPNAME}"
	CreateShortCut "$SMPROGRAMS\mtp-target\Launch dedicated server.lnk" "$INSTDIR\mtp_target_service.exe" "" "$INSTDIR\mtp_target_service.exe" 0

SectionEnd


Section -FinishSection

	WriteRegStr HKLM "Software\${APPNAME}" "" "$INSTDIR"
	WriteRegStr HKCU "Software\${APPNAME}" "version" "${APPVERSION}"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "DisplayName" "${APPNAME}"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "UninstallString" "$INSTDIR\uninstall.exe"
	WriteUninstaller "$INSTDIR\uninstall.exe"

SectionEnd

; Modern install component descriptions
!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
	!insertmacro MUI_DESCRIPTION_TEXT ${Section1} "Client data"
	!insertmacro MUI_DESCRIPTION_TEXT ${Section2} "Server"
!insertmacro MUI_FUNCTION_DESCRIPTION_END

;Uninstall section
Section Uninstall

	;Remove from registry...
	DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}"
	DeleteRegKey HKLM "SOFTWARE\${APPNAME}"

	Delete "$INSTDIR\*.*" 
	Delete "$INSTDIR\cache\*.*" 
	RmDir "$INSTDIR\cache"
	Delete "$INSTDIR\data\*.*" 
	RmDir "$INSTDIR\data"
	Delete "$INSTDIR\replay\*.*" 
	RmDir "$INSTDIR\replay"

	;Delete Start Menu Shortcuts
	Delete "$SMPROGRAMS\${APPNAME}\*.*"
  RmDir "$SMPROGRAMS\${APPNAME}"

  RMDir "$INSTDIR"
  DeleteRegKey /ifempty HKCU "Software\${APPNAME}"

SectionEnd

BrandingText "plopida"

; eof