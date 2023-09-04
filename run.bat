@echo off

pushd build
call ..\deps\ctime\ctime -begin hsc-run.ctime
call hsc.exe ..\main.lox
call ..\deps\ctime\ctime -end hsc-run.ctime %ERRORLEVEL%
popd
