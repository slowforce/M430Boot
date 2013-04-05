@echo off
rem delete hex file and json file
del ..\Debug\*.hex
del ..\Debug\*.json
rem get out file name
for /R ..\Debug %%i in (*.out) DO (
set fileName=%%~ni
)
echo %fileName%
hex430 -i ..\Debug\%fileName%.out -o ..\Debug\%fileName%.hex -order MS -romwidth 16
lua ..\scripts\release.lua