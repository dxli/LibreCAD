@echo off
setlocal enabledelayedexpansion

rem This batch file sets up the building environment for LibreCAD master branch on Windows.
rem It installs Chocolatey if not present, uses Chocolatey to install git, aqt, visualstudio2022buildtools, visualstudio2022-workload-vctools, and boost-msvc-14.3.
rem Uses aqt to install Qt6 with MSVC.
rem Assumptions:
rem - Run this batch as Administrator.
rem - Adjust versions if needed.
rem - Directories: C:\Qt for Qt, C:\local\boost_1_89_0 for Boost, current dir for repo.

rem Check if Chocolatey is installed
where choco >nul 2>nul
if %errorlevel% neq 0 (
    echo Installing Chocolatey...
    powershell -NoProfile -ExecutionPolicy Bypass -Command "[System.Net.ServicePointManager]::SecurityProtocol = [System.Net.ServicePointManager]::SecurityProtocol -bor 3072; iex ((New-Object System.Net.WebClient).DownloadString('https://community.chocolatey.org/install.ps1'))"
    if %errorlevel% neq 0 (
        echo Failed to install Chocolatey.
        pause
        exit /b 1
    )
    set PATH=%PATH%;%ALLUSERSPROFILE%\chocolatey\bin
)

rem Install git, aqt, visualstudio2022buildtools, visualstudio2022-workload-vctools, and boost via Chocolatey
choco install git -y
if %errorlevel% neq 0 (
    echo Failed to install git.
    pause
    exit /b 1
)
choco install aqt -y
if %errorlevel% neq 0 (
    echo Failed to install aqt.
    pause
    exit /b 1
)
choco install visualstudio2022buildtools -y
if %errorlevel% neq 0 (
    echo Failed to install visualstudio2022buildtools.
    pause
    exit /b 1
)
choco install visualstudio2022-workload-vctools -y
if %errorlevel% neq 0 (
    echo Failed to install visualstudio2022-workload-vctools.
    pause
    exit /b 1
)
choco install boost-msvc-14.3 -y
if %errorlevel% neq 0 (
    echo Failed to install boost-msvc-14.3.
    pause
    exit /b 1
)

rem Install Qt 6.10.0 with MSVC 2022 64-bit using aqt
if not exist C:\Qt\6.10.0\msvc2022_64 (
    echo Installing Qt 6.10.0 MSVC 2022 64-bit...
    aqt install-qt --outputdir C:\Qt windows desktop 6.10.0 win64_msvc2022_64
    if %errorlevel% neq 0 (
        echo Failed to install Qt.
        pause
        exit /b 1
    )
) else (
    echo Qt already installed.
)

rem Set Qt path
set QT_DIR=C:\Qt\6.10.0\msvc2022_64
set PATH=%QT_DIR%\bin;%PATH%

rem Set Boost path (adjust version if the installed version differs)
set BOOST_VERSION=1_89_0
set BOOST_DIR=C:\local\boost_%BOOST_VERSION%

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

rem Set up MSVC environment
call "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if %errorlevel% neq 0 (
    echo Failed to set up MSVC environment.
    pause
    exit /b 1
)

rem Create or append to custom.pro in librecad\src
set CUSTOM_PRO=librecad\src\custom.pro
if not exist "%CUSTOM_PRO%" (
    echo Creating custom.pro
    > "%CUSTOM_PRO%" echo # Custom settings for build
)
echo. >> "%CUSTOM_PRO%"
echo BOOST_DIR = %BOOST_DIR:\=/% >> "%CUSTOM_PRO%"
echo BOOST_LIB = libboost_program_options-vc143-mt-x64-1_89 >> "%CUSTOM_PRO%"  rem Adjust Boost lib name if needed, using libboost_ for static
echo BOOST_LIBDIR = %BOOST_DIR:\=/%/lib64-msvc-14.3 >> "%CUSTOM_PRO%"

rem Run qmake
qmake librecad.pro
if %errorlevel% neq 0 (
    echo qmake failed. Check Qt installation and paths.
    pause
    exit /b 1
)

rem Run nmake for MSVC
nmake release
if %errorlevel% neq 0 (
    echo Build failed.
    pause
    exit /b 1
)

echo Build completed successfully. Executable is in windows\release\librecad.exe
echo To run, copy required DLLs from %QT_DIR%\bin to the executable directory.
echo Required DLLs include: Qt6Core.dll, Qt6Gui.dll, Qt6Widgets.dll, etc.

pause
endlocal