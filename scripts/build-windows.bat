call set-windows-env.bat

rem qmake6.exe librecad.pro -r -spec win32-g++
cd ..
cd 
dir 
mkdir build

cd build
cmake.exe .. -G "Visual Studio 17 2022"
cmake --build . --config Release
rem msbuild /p:Configuration=${{ env.BUILD_CONFIGURATION }} /p:OutDir=${{ runner.temp }}\build\ "${{ env.SOLUTION_FILE_PATH }}\QtStockV3.sln"


dir *.exe
if NOT exist LibreCAD.exe (
	echo "Building windows\LibreCAD.exe failed!"
	exit /b /1
)

set
windeployqt6.exe --release LibreCAD--compiler-runtime
cd ..
cd


call build-win-setup.bat
