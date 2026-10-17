@echo off
cd generated
windeployqt6.exe --release --compiler-runtime Release\LibreCAD.exe
if errorlevel 1 exit /b 1
cd ..
xcopy /E /Y /I generated\Release windows
if errorlevel 1 exit /b 1
echo reached>reached.txt
exit /b 0
