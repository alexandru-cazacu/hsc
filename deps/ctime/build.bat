@echo off

REM Debug build:
REM cl -Zi ctime.c /link winmm.lib

REM Release build:
cl /nologo -O2 ctime.c /link winmm.lib
