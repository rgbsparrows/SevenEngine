if exist Intermediate if "%~1" == "-force" rmdir /s /q Intermediate
if not exist Intermediate md Intermediate
cd Intermediate
cmake ..\CMakeFile
cd ..
