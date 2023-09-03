@echo off

pushd build
call ..\deps\ctime\ctime -begin hsc-run.ctime
call hsc.exe ..\main.hs
call ..\deps\ctime\ctime -end hsc-run.ctime %ERRORLEVEL%
popd
