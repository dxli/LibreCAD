call set-windows-env.bat

rem qmake6.exe librecad.pro -r -spec win32-g++
cd
mkdir build

pushed build
cmake.exe .. -G "Visual Studio 17 2022"
cmake --build . --config Release

dir *.exe
if NOT exist LibreCAD.exe (
	echo "Building windows\LibreCAD.exe failed!"
	exit /b /1
)

set
windeployqt6.exe --release LibreCAD--compiler-runtime
popd
cd


call build-win-setup.bat
