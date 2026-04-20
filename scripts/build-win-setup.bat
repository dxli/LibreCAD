call set-windows-env.bat

if _%LC_NSIS_FILE%==_ (
    set LC_NSIS_FILE=nsis-msvc.nsi
)

if _%LC_ARCH%==_ (
    set LC_ARCH=AMD64
)

pushd postprocess-windows
makensis.exe /X"SetCompressor /FINAL lzma" /V4 /D%LC_ARCH% %LC_NSIS_FILE%
popd
