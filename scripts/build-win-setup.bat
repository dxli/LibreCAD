call set-windows-env.bat

if _%LC_NSIS_FILE%==_ (
    set LC_NSIS_FILE=nsis-msvc.nsi
)

if _%LC_ARCH%==_ (
    set LC_ARCH=AMD64
)

pushd postprocess-windows

rem Build NSIS flags with architecture
set NSIS_FLAGS=/X"SetCompressor /FINAL lzma" /V4 /D%LC_ARCH%

rem Pass SCMREVISION and VIProductVersion if defined
if NOT "_%SCMREVISION%"=="_" (
    set NSIS_FLAGS=!NSIS_FLAGS! /DSCMREVISION="%SCMREVISION%"
)
if NOT "_%VIProductVersion%"=="_" (
    set NSIS_FLAGS=!NSIS_FLAGS! /DVIProductVersion="%VIProductVersion%"
)

makensis.exe !NSIS_FLAGS! %LC_NSIS_FILE%
popd
