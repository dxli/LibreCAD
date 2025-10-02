@echo off
setlocal enabledelayedexpansion

rem This batch file sets up the building environment for LibreCAD master branch on Windows.
rem Assumptions:
rem - Qt is installed with MinGW compiler (e.g., Qt 5.15 or compatible, MinGW 8.1 or similar).
rem - Boost library is downloaded and extracted (e.g., from https://www.boost.org/users/download/).
rem - Git is installed (if not, install via Chocolatey or manually).
rem - Run this batch from a directory where you want to clone the repo (e.g., C:\dev).
rem - Adjust the paths below to match your installation.

rem Set your Qt installation path (adjust version and MinGW bitness as needed)
set QT_DIR=C:\Qt\5.15.2\mingw81_64
set MINGW_DIR=%QT_DIR%\..\..\Tools\mingw810_64

rem Set your Boost extraction path (adjust version as needed)
set BOOST_DIR=C:\dev\boost_1_85_0

rem Add Qt and MinGW to PATH
set PATH=%QT_DIR%\bin;%MINGW_DIR%\bin;%PATH%

rem Check if git is available
where git >nul 2>nul
if %errorlevel% neq 0 (
    echo Git not found. Please install Git manually or via Chocolatey (choco install git).
    pause
    exit /b 1
)

rem Clone the repository if not exists
if not exist LibreCAD (
    git clone https://github.com/LibreCAD/LibreCAD.git --branch master
    if %errorlevel% neq 0 (
        echo Failed to clone repository.
        pause
        exit /b 1
    )
) else (
    echo Repository already exists. Pulling latest changes.
    cd LibreCAD
    git pull
    cd ..
)

rem Navigate to the source directory
cd LibreCAD

rem Create or append to custom.pro in librecad\src
set CUSTOM_PRO=librecad\src\custom.pro
if not exist "%CUSTOM_PRO%" (
    echo Creating custom.pro
    > "%CUSTOM_PRO%" echo # Custom settings for build
)
echo. >> "%CUSTOM_PRO%"
echo BOOST_DIR = %BOOST_DIR:\=/% >> "%CUSTOM_PRO%"
echo BOOST_LIB = boost_program_options-mgw81-mt-x64-1_85 >> "%CUSTOM_PRO%"  rem Adjust Boost lib name if needed
echo BOOST_LIBDIR = %BOOST_DIR:\=/% >> "%CUSTOM_PRO%"

rem Run qmake
qmake librecad.pro
if %errorlevel% neq 0 (
    echo qmake failed. Check Qt installation and paths.
    pause
    exit /b 1
)

rem Run make (use -j for parallel builds if desired)
mingw32-make release
if %errorlevel% neq 0 (
    echo Build failed.
    pause
    exit /b 1
)

echo Build completed successfully. Executable is in windows\release\librecad.exe
echo To run, copy required DLLs from %QT_DIR%\bin and %MINGW_DIR%\bin to the executable directory.
echo Required DLLs include: libgcc_s_seh-1.dll, libstdc++-6.dll, libwinpthread-1.dll, Qt5Core.dll, Qt5Gui.dll, Qt5Widgets.dll, etc.

pause
endlocal


