@echo off
if not exist Intermediate\SevenEngine.sln call GerneateProjectFile.bat
if exist Intermediate\SevenEngine.sln Intermediate\SevenEngine.sln else echo generate sln failed
