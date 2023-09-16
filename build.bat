@echo off

if not exist build mkdir build

set opts=-FC -GR- -EHa- -nologo -O2
set code=%cd%/src

pushd build
call ..\deps\ctime\ctime -begin hsc.ctime
cl %opts% %code%/main.c -Fehsc.exe /link -incremental:no
call ..\deps\ctime\ctime -end hsc.ctime %ERRORLEVEL%
popd
