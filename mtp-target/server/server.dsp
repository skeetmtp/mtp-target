# Microsoft Developer Studio Project File - Name="Server" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Server - Win32 DebugFast
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "server.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "server.mak" CFG="Server - Win32 DebugFast"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Server - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Server - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "Server - Win32 ReleaseDebug" (based on "Win32 (x86) Application")
!MESSAGE "Server - Win32 DebugFast" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Server - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /D "NDEBUG" /D "_WINDOWS" /D "WIN32" /D "_MBCS" /D "MTPT_SERVER" /Yu"stdpch.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40c /d "NDEBUG"
# ADD RSC /l 0x40c /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 opcode.lib ode.lib nlmisc_r.lib nlnet_r.lib zlib.lib freetype.lib lua.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib:"libc.lib" /out:"Release/mtp_target_service.exe"

!ELSEIF  "$(CFG)" == "Server - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /Zi /Od /D "__STL_DEBUG" /D "_DEBUG" /D "_WINDOWS" /D "WIN32" /D "_MBCS" /D "MTPT_SERVER" /Yu"stdpch.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40c /d "_DEBUG"
# ADD RSC /l 0x40c /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ode.lib opcode.lib nlmisc_d.lib nlnet_d.lib zlib.lib freetype.lib lua.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"libc.lib" /out:"Debug/mtp_target_service.exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "Server - Win32 ReleaseDebug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Server___Win32_ReleaseDebug"
# PROP BASE Intermediate_Dir "Server___Win32_ReleaseDebug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseDebug"
# PROP Intermediate_Dir "ReleaseDebug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GR /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /Zi /O2 /D "NDEBUG" /D "NL_RELEASE_DEBUG" /D "_WINDOWS" /D "WIN32" /D "_MBCS" /D "MTPT_SERVER" /Yu"stdpch.h" /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40c /d "NDEBUG"
# ADD RSC /l 0x40c /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 nlmisc_r.lib nlnet_r.lib ode.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib:"libc.lib"
# ADD LINK32 ode.lib opcode.lib nlmisc_rd.lib nlnet_rd.lib zlib.lib freetype.lib lua.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"libc.lib" /out:"ReleaseDebug/mtp_target_service.exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "Server - Win32 DebugFast"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Server___Win32_DebugFast"
# PROP BASE Intermediate_Dir "Server___Win32_DebugFast"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DebugFast"
# PROP Intermediate_Dir "DebugFast"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /Gm /GR /GX /Zi /Od /D "__STL_DEBUG" /D "_WINDOWS" /D "WIN32" /D "NDEBUG" /D "_MBCS" /Yu"stdpch.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /Zi /Od /Ob1 /D "_DEBUG" /D "NL_DEBUG_FAST" /D "_WINDOWS" /D "WIN32" /D "_MBCS" /D "MTPT_SERVER" /Yu"stdpch.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40c /d "_DEBUG"
# ADD RSC /l 0x40c /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 dTriList_rd.lib nlmisc_d.lib nlnet_d.lib ode.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"libc.lib" /pdbtype:sept
# ADD LINK32 nlmisc_df.lib nlnet_df.lib ode.lib opcode.lib zlib.lib freetype.lib lua.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"libc.lib" /out:"DebugFast/mtp_target_service.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "Server - Win32 Release"
# Name "Server - Win32 Debug"
# Name "Server - Win32 ReleaseDebug"
# Name "Server - Win32 DebugFast"
# Begin Group "entity"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Src\bot.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\bot.h
# End Source File
# Begin Source File

SOURCE=.\Src\client.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\client.h
# End Source File
# Begin Source File

SOURCE=.\Src\entity.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\entity.h
# End Source File
# Begin Source File

SOURCE=.\Src\entity_lua_proxy.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\entity_lua_proxy.h
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

SOURCE=.\Src\network.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\network.h
# End Source File
# End Group
# Begin Group "level"

# PROP Default_Filter ""
# Begin Group "editableElement"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\common\editable_element_common.cpp
# End Source File
# Begin Source File

SOURCE=..\common\editable_element_common.h
# End Source File
# Begin Source File

SOURCE=.\src\module.cpp
# End Source File
# Begin Source File

SOURCE=.\src\module.h
# End Source File
# Begin Source File

SOURCE=..\common\module_common.cpp
# End Source File
# Begin Source File

SOURCE=..\common\module_common.h
# End Source File
# Begin Source File

SOURCE=.\Src\module_lua_proxy.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\module_lua_proxy.h
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

SOURCE=.\Src\level_lua_proxy.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\level_lua_proxy.h
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

SOURCE=.\Src\lua_engine.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\lua_engine.h
# End Source File
# Begin Source File

SOURCE=..\common\lua_nel.cpp
# End Source File
# Begin Source File

SOURCE=..\common\lua_nel.h
# End Source File
# Begin Source File

SOURCE=..\common\lua_utility.cpp
# End Source File
# Begin Source File

SOURCE=..\common\lua_utility.h
# End Source File
# Begin Source File

SOURCE=..\common\luna.h
# End Source File
# Begin Source File

SOURCE=..\common\lunar.h
# End Source File
# End Group
# Begin Group "session"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Src\ending_session_state.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\ending_session_state.h
# End Source File
# Begin Source File

SOURCE=.\Src\running_session_state.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\running_session_state.h
# End Source File
# Begin Source File

SOURCE=.\Src\session_manager.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\session_manager.h
# End Source File
# Begin Source File

SOURCE=.\Src\session_state.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\session_state.h
# End Source File
# Begin Source File

SOURCE=.\Src\waiting_clients_session_state.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\waiting_clients_session_state.h
# End Source File
# Begin Source File

SOURCE=.\Src\waiting_ready_session_state.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\waiting_ready_session_state.h
# End Source File
# Begin Source File

SOURCE=.\Src\waiting_start_session_state.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\waiting_start_session_state.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\common\constant.h
# End Source File
# Begin Source File

SOURCE=.\Src\main.cpp

!IF  "$(CFG)" == "Server - Win32 Release"

# ADD CPP /FAs

!ELSEIF  "$(CFG)" == "Server - Win32 Debug"

!ELSEIF  "$(CFG)" == "Server - Win32 ReleaseDebug"

# ADD BASE CPP /FAs
# ADD CPP /FAs

!ELSEIF  "$(CFG)" == "Server - Win32 DebugFast"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Src\main.h
# End Source File
# Begin Source File

SOURCE=..\mtp_target.txt
# End Source File
# Begin Source File

SOURCE=.\mtp_target_service.cfg
# End Source File
# Begin Source File

SOURCE=.\mtp_target_service_default.cfg
# End Source File
# Begin Source File

SOURCE=..\common\pause_flag.h
# End Source File
# Begin Source File

SOURCE=.\Src\physics.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\physics.h
# End Source File
# Begin Source File

SOURCE=..\common\singleton.h
# End Source File
# Begin Source File

SOURCE=.\Src\stdpch.cpp
# ADD CPP /Yc"stdpch.h"
# End Source File
# Begin Source File

SOURCE=.\Src\stdpch.h
# End Source File
# Begin Source File

SOURCE=..\www\todo.txt
# End Source File
# Begin Source File

SOURCE=.\Src\variables.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\variables.h
# End Source File
# Begin Source File

SOURCE=.\Src\welcome.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\welcome.h
# End Source File
# End Target
# End Project
