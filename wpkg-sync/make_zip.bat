if exist wpkg-sync.zip del /F wpkg-sync.zip

mkdir wpkg-sync

copy release\wpkg-sync.exe wpkg-sync\
copy wpkg-sync_default.cfg wpkg-sync\wpkg-sync.cfg
copy THANKS wpkg-sync\
copy NEWS wpkg-sync\
copy README wpkg-sync\
copy COPYING wpkg-sync\
copy C:\code\wpkg\bin\stlport_vc645.dll wpkg-sync\

"C:\Program Files\WinZip\WINZIP32.EXE" -min -a -ex wpkg-sync.zip wpkg-sync

rmdir /Q /S wpkg-sync
