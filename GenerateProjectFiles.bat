@echo off

if not exist "%~dp0Build\BatchFiles\GenerateProjectFiles.bat" goto Error_BatchFileInWrongLocation
call "%~dp0Build\BatchFiles\GenerateProjectFiles.bat" %* -visualstudio2019
exit /B %ERRORLEVEL%

:Error_BatchFileInWrongLocation
echo GenerateProjectFiles ERROR: The batch file does not appear to be located in the root UE4 directory.  This script must be run from within that directory.
pause
exit /B 1
