# Microsoft Developer Studio Project File - Name="Client" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Client - Win32 DebugFast
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "client.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "client.mak" CFG="Client - Win32 DebugFast"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Client - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Client - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "Client - Win32 ReleaseDebug" (based on "Win32 (x86) Application")
!MESSAGE "Client - Win32 DebugFast" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Client - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /I "../../RealTimeFairy/include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdpch.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40c /d "NDEBUG"
# ADD RSC /l 0x40c /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 nlmisc_r.lib nlnet_r.lib nl3d_r.lib lua.lib jpeg.lib freetype.lib zlib.lib xml2.lib fmodvc.lib user32.lib gdi32.lib advapi32.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib:"libcmt.lib" /out:"Release/mtp-target.exe" /libpath:"../../RealTimeFairy/extern/lib"

!ELSEIF  "$(CFG)" == "Client - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /Zi /Od /I "../../RealTimeFairy/include" /D "__STL_DEBUG" /D "_WINDOWS" /D "WIN32" /D "_DEBUG" /D "_MBCS" /Yu"stdpch.h" /FD /GZ /Zm200 /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40c /d "_DEBUG"
# ADD RSC /l 0x40c /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 nlmisc_d.lib nlnet_d.lib nl3d_d.lib lua.lib jpeg.lib freetype.lib zlib.lib xml2.lib fmodvc.lib user32.lib gdi32.lib advapi32.lib /nologo /subsystem:windows /debug /machine:I386 /out:"Debug/mtp-target.exe" /pdbtype:sept /libpath:"../../RealTimeFairy/extern/lib"

!ELSEIF  "$(CFG)" == "Client - Win32 ReleaseDebug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Client___Win32_ReleaseDebug"
# PROP BASE Intermediate_Dir "Client___Win32_ReleaseDebug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseDebug"
# PROP Intermediate_Dir "ReleaseDebug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GR /GX /O2 /I "../../RealTimeFairy/include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /Zi /O2 /I "../../RealTimeFairy/include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "NL_RELEASE_DEBUG" /Yu"stdpch.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40c /d "NDEBUG"
# ADD RSC /l 0x40c /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 nlmisc_r.lib nlnet_r.lib fmodvc.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib libjpeg.lib libpng.lib zlib.lib d3d8.lib vfw32.lib dxguid.lib dinput8.lib d3dx8.lib fmodvc.lib freetype211ST.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib:"libc.lib" /libpath:"../../RealTimeFairy/extern/lib"
# ADD LINK32 nlmisc_rd.lib nlnet_rd.lib nl3d_rd.lib lua.lib jpeg.lib freetype.lib zlib.lib xml2.lib fmodvc.lib user32.lib gdi32.lib advapi32.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"libc.lib" /out:"ReleaseDebug/mtp-target.exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "Client - Win32 DebugFast"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Client___Win32_DebugFast"
# PROP BASE Intermediate_Dir "Client___Win32_DebugFast"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DebugFast"
# PROP Intermediate_Dir "DebugFast"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GR /GX /Zi /Od /I "../../RealTimeFairy/include" /D "__STL_DEBUG" /D "_WINDOWS" /D "WIN32" /D "_DEBUG" /D "_MBCS" /Yu"stdpch.h" /FD /GZ /Zm200 /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /Zi /Od /Ob1 /I "../../RealTimeFairy/include" /D "_WINDOWS" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "NL_DEBUG_FAST" /Yu"stdpch.h" /FD /GZ /Zm200 /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40c /d "_DEBUG"
# ADD RSC /l 0x40c /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 nlmisc_d.lib nlnet_d.lib nl3d_d.lib freetype211.lib fmodvc.lib user32.lib gdi32.lib advapi32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept /libpath:"../../RealTimeFairy/extern/lib"
# ADD LINK32 nlmisc_df.lib nlnet_df.lib nl3d_df.lib lua.lib jpeg.lib freetype.lib zlib.lib xml2.lib fmodvc.lib user32.lib gdi32.lib advapi32.lib /nologo /subsystem:windows /debug /machine:I386 /out:"DebugFast/mtp-target.exe" /pdbtype:sept /libpath:"../../RealTimeFairy/extern/lib"

!ENDIF 

# Begin Target

# Name "Client - Win32 Release"
# Name "Client - Win32 Debug"
# Name "Client - Win32 ReleaseDebug"
# Name "Client - Win32 DebugFast"
# Begin Group "entity"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Src\entity.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\entity.h
# End Source File
# Begin Source File

SOURCE=.\Src\entity_manager.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\entity_manager.h
# End Source File
# End Group
# Begin Group "network"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\common\custom_floating_point.cpp
# End Source File
# Begin Source File

SOURCE=..\common\custom_floating_point.h
# End Source File
# Begin Source File

SOURCE=.\Src\net_callbacks.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\net_callbacks.h
# End Source File
# Begin Source File

SOURCE=..\common\net_message.cpp
# End Source File
# Begin Source File

SOURCE=..\common\net_message.h
# End Source File
# Begin Source File

SOURCE=.\Src\network_task.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\network_task.h
# End Source File
# End Group
# Begin Group "level"

# PROP Default_Filter ""
# Begin Group "editable_element"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\common\editable_element_common.cpp
# End Source File
# Begin Source File

SOURCE=..\common\editable_element_common.h
# End Source File
# Begin Source File

SOURCE=.\Src\module.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\module.h
# End Source File
# Begin Source File

SOURCE=..\common\module_common.cpp
# End Source File
# Begin Source File

SOURCE=..\common\module_common.h
# End Source File
# Begin Source File

SOURCE=.\src\start_point.cpp
# End Source File
# Begin Source File

SOURCE=.\src\start_point.h
# End Source File
# Begin Source File

SOURCE=..\common\start_point_common.cpp
# End Source File
# Begin Source File

SOURCE=..\common\start_point_common.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Src\level.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\level.h
# End Source File
# Begin Source File

SOURCE=.\Src\level_manager.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\level_manager.h
# End Source File
# Begin Source File

SOURCE=..\common\load_mesh.cpp
# End Source File
# Begin Source File

SOURCE=..\common\load_mesh.h
# End Source File
# End Group
# Begin Group "lua"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\common\lua_nel.cpp
# End Source File
# Begin Source File

SOURCE=..\common\lua_utility.cpp
# End Source File
# Begin Source File

SOURCE=..\common\lua_utility.h
# End Source File
# End Group
# Begin Group "task"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Src\3d_task.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\3d_task.h
# End Source File
# Begin Source File

SOURCE=.\Src\background_task.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\background_task.h
# End Source File
# Begin Source File

SOURCE=.\Src\chat_task.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\chat_task.h
# End Source File
# Begin Source File

SOURCE=.\Src\config_file_task.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\config_file_task.h
# End Source File
# Begin Source File

SOURCE=.\Src\editor_task.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\editor_task.h
# End Source File
# Begin Source File

SOURCE=.\Src\game_task.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\game_task.h
# End Source File
# Begin Source File

SOURCE=.\Src\gui_task.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\gui_task.h
# End Source File
# Begin Source File

SOURCE=.\Src\hud_task.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\hud_task.h
# End Source File
# Begin Source File

SOURCE=.\Src\intro_task.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\intro_task.h
# End Source File
# Begin Source File

SOURCE=.\Src\lens_flare_task.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\lens_flare_task.h
# End Source File
# Begin Source File

SOURCE=.\Src\resource_manager.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\resource_manager.h
# End Source File
# Begin Source File

SOURCE=.\Src\score_task.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\score_task.h
# End Source File
# Begin Source File

SOURCE=.\Src\sky_task.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\sky_task.h
# End Source File
# Begin Source File

SOURCE=.\Src\swap_3d_task.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\swap_3d_task.h
# End Source File
# Begin Source File

SOURCE=.\Src\time_task.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\time_task.h
# End Source File
# Begin Source File

SOURCE=.\src\water_task.cpp
# End Source File
# Begin Source File

SOURCE=.\src\water_task.h
# End Source File
# End Group
# Begin Group "gui"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Src\gui.h
# End Source File
# Begin Source File

SOURCE=.\Src\gui_bin.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\gui_bin.h
# End Source File
# Begin Source File

SOURCE=.\Src\gui_bitmap.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\gui_bitmap.h
# End Source File
# Begin Source File

SOURCE=.\Src\gui_box.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\gui_box.h
# End Source File
# Begin Source File

SOURCE=.\Src\gui_button.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\gui_button.h
# End Source File
# Begin Source File

SOURCE=.\Src\gui_container.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\gui_container.h
# End Source File
# Begin Source File

SOURCE=.\Src\gui_event_behaviour.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\gui_event_behaviour.h
# End Source File
# Begin Source File

SOURCE=.\Src\gui_frame.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\gui_frame.h
# End Source File
# Begin Source File

SOURCE=.\Src\gui_mouse_listener.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\gui_mouse_listener.h
# End Source File
# Begin Source File

SOURCE=.\Src\gui_multiline_text.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\gui_multiline_text.h
# End Source File
# Begin Source File

SOURCE=.\Src\gui_object.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\gui_object.h
# End Source File
# Begin Source File

SOURCE=.\Src\gui_progress_bar.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\gui_progress_bar.h
# End Source File
# Begin Source File

SOURCE=.\Src\gui_quad.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\gui_quad.h
# End Source File
# Begin Source File

SOURCE=.\Src\gui_scale.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\gui_scale.h
# End Source File
# Begin Source File

SOURCE=.\Src\gui_scrollbar.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\gui_scrollbar.h
# End Source File
# Begin Source File

SOURCE=.\Src\gui_spacer.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\gui_spacer.h
# End Source File
# Begin Source File

SOURCE=.\Src\gui_spg.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\gui_spg.h
# End Source File
# Begin Source File

SOURCE=.\Src\gui_stretched_quad.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\gui_stretched_quad.h
# End Source File
# Begin Source File

SOURCE=.\Src\gui_text.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\gui_text.h
# End Source File
# Begin Source File

SOURCE=.\Src\gui_xml.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\gui_xml.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Src\camera.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\camera.h
# End Source File
# Begin Source File

SOURCE=..\common\constant.h
# End Source File
# Begin Source File

SOURCE=.\Src\controler.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\controler.h
# End Source File
# Begin Source File

SOURCE=.\favicon.ico
# End Source File
# Begin Source File

SOURCE=.\Src\font_manager.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\font_manager.h
# End Source File
# Begin Source File

SOURCE=.\Src\global.h
# End Source File
# Begin Source File

SOURCE=.\Src\interface.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\interface.h
# End Source File
# Begin Source File

SOURCE=.\Src\interpolator.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\interpolator.h
# End Source File
# Begin Source File

SOURCE=.\Src\login_client.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\login_client.h
# End Source File
# Begin Source File

SOURCE=.\Src\main.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\mouse_listener.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\mouse_listener.h
# End Source File
# Begin Source File

SOURCE=.\mtp_target.cfg
# End Source File
# Begin Source File

SOURCE=.\Src\mtp_target.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\mtp_target.h
# End Source File
# Begin Source File

SOURCE=.\mtp_target_default.cfg
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\Script1.rc
# End Source File
# Begin Source File

SOURCE=..\common\singleton.h
# End Source File
# Begin Source File

SOURCE=.\Src\sound_manager.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\sound_manager.h
# End Source File
# Begin Source File

SOURCE=.\Src\stdpch.cpp
# ADD CPP /Yc"stdpch.h"
# End Source File
# Begin Source File

SOURCE=.\Src\stdpch.h
# End Source File
# Begin Source File

SOURCE=.\Src\task.h
# End Source File
# Begin Source File

SOURCE=.\Src\task_manager.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\task_manager.h
# End Source File
# Begin Source File

SOURCE=..\www\todo.txt
# End Source File
# End Target
# End Project
