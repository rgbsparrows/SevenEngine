if exist Build if "%~1" == "-force" rmdir /s /q Build
if not exist Build md Build
cd Build
cmake ..\CMakeFile
cd ..
