@echo off
cd generated
windeployqt6.exe --release --compiler-runtime ARM64\Release\LibreCAD.exe
cd ..
xcopy /E /Y /I generated\ARM64\Release windows
echo reached>reached.txt
exit /b 0
