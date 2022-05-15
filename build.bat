@echo off

if not exist build mkdir build

set opts=-FC -GR- -EHa- -nologo -Zi
set code=%cd%
pushd build
call ..\deps\ctime\ctime -begin hsc.ctime
cl %opts% %code%/hsc.c -Fehsc.exe
call ..\deps\ctime\ctime -end hsc.ctime %ERRORLEVEL%
popd
