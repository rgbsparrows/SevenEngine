@echo off
call GerneateProjectFile.bat
if exist Intermediate\SevenEngine.sln Intermediate\SevenEngine.sln else echo generate sln failed
