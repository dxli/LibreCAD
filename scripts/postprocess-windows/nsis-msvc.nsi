; nsis-msvc.nsi – Modernized NSIS installer for LibreCAD (MSVC/Qt builds, 2026 style)
; All .qm translation files go into resources/qm/ (no separate translations/ folder)

SetCompressor /SOLID lzma
Unicode True

!include "MUI2.nsh"
!include "LogicLib.nsh"
!include "WinVer.nsh"

;───────────────────────────────────────────────
; Custom overrides (optional: create custom.nsh to override Qt paths etc.)
!include /NONFATAL "custom.nsh"

;───────────────────────────────────────────────
; SCMREVISION priority: cmd-line > generated file > fallback
!ifdef SCMREVISION
  ; /DSCMREVISION=... passed to makensis
!else
  !include /NONFATAL "generated_scmrev.nsh"
  !ifndef SCMREVISION
    !define SCMREVISION "2.2.1.x"   ; update fallback as needed
  !endif
!endif

;───────────────────────────────────────────────
; Core definitions
!ifndef APPNAME
  !define APPNAME "LibreCAD"
!endif

!define COMPANY     "LibreCAD Team"
!define UNINSTKEY   "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}"
!define MUI_ICON    "..\..\librecad\res\main\librecad.ico"
!define MUI_UNICON  "..\..\librecad\res\main\uninstall.ico"

; Dynamic architecture
!ifdef AMD64
  !define ARCH_SUFFIX "x64"
  InstallDir "$PROGRAMFILES64\${APPNAME}"
!else
  !define ARCH_SUFFIX "x86"
  InstallDir "$PROGRAMFILES\${APPNAME}"
!endif

Name "${APPNAME} ${SCMREVISION}"
OutFile "..\..\generated\LibreCAD-${SCMREVISION}-Windows-${ARCH_SUFFIX}.exe"

RequestExecutionLevel admin

; Version info
VIProductVersion    "2.2.1.0"   ; adjust dotted version if needed
VIAddVersionKey     "ProductName"     "${APPNAME}"
VIAddVersionKey     "FileVersion"     "${SCMREVISION}"
VIAddVersionKey     "ProductVersion"  "${SCMREVISION}"
VIAddVersionKey     "FileDescription" "${APPNAME} Installer"
VIAddVersionKey     "LegalCopyright"  "© ${COMPANY}"
VIAddVersionKey     "CompanyName"     "${COMPANY}"

;───────────────────────────────────────────────
; UI settings
!define MUI_ABORTWARNING
!define MUI_FINISHPAGE_RUN          "$INSTDIR\LibreCAD.exe"
!define MUI_FINISHPAGE_RUN_FUNCTION "LaunchLink"
!define MUI_FINISHPAGE_RUN_CHECKED

; Pages
!insertmacro MUI_PAGE_LICENSE       "../../licenses/gpl-2.0.txt"
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_COMPONENTS
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES

; Languages (installer UI)
!insertmacro MUI_LANGUAGE "English"
!insertmacro MUI_LANGUAGE "French"
!insertmacro MUI_LANGUAGE "German"
!insertmacro MUI_LANGUAGE "Spanish"
!insertmacro MUI_LANGUAGE "SimpChinese"
!insertmacro MUI_LANGUAGE "TradChinese"
!insertmacro MUI_LANGUAGE "Japanese"
!insertmacro MUI_LANGUAGE "Korean"
!insertmacro MUI_LANGUAGE "Italian"
!insertmacro MUI_LANGUAGE "Russian"
!insertmacro MUI_LANGUAGE "Polish"
!insertmacro MUI_LANGUAGE "PortugueseBR"
!insertmacro MUI_LANGUAGE "Czech"
!insertmacro MUI_LANGUAGE "Turkish"

;───────────────────────────────────────────────
; Qt paths (override in custom.nsh if your setup differs)
!ifndef Qt_Dir
  !define Qt_Dir "C:\Qt"
!endif
!ifndef Qt_Version
  !define Qt_Version "5.15.2"   ; change to 6.x.x if using Qt6
!endif
!ifndef MSVC_Ver
  !ifdef AMD64
    !define MSVC_Ver "msvc2019_64"
  !else
    !define MSVC_Ver "msvc2019"
  !endif
!endif

!define QT_BIN_DIR       "${Qt_Dir}\${Qt_Version}\${MSVC_Ver}\bin"
!define QT_PLUGINS_DIR   "${Qt_Dir}\${Qt_Version}\${MSVC_Ver}\plugins"
!define QT_TRANSLATIONS  "${Qt_Dir}\${Qt_Version}\${MSVC_Ver}\translations"

;───────────────────────────────────────────────
; Components
Section "LibreCAD Core" SecMain
  SectionIn RO

  !ifdef AMD64
    SetRegView 64
  !else
    SetRegView 32
  !endif

  SetOutPath "$INSTDIR"

  ; Main files (assume windeployqt already copied Qt DLLs/plugins here)
  File /r /x "*.pdb" "..\..\windows\*.*"

  ; Key Qt plugins (fallback, non-fatal)
  SetOutPath "$INSTDIR\platforms"
  File /nonfatal "${QT_PLUGINS_DIR}\platforms\qwindows.dll"

  SetOutPath "$INSTDIR\imageformats"
  File /nonfatal "${QT_PLUGINS_DIR}\imageformats\qgif.dll"
  File /nonfatal "${QT_PLUGINS_DIR}\imageformats\qico.dll"
  File /nonfatal "${QT_PLUGINS_DIR}\imageformats\qjpeg.dll"
  File /nonfatal "${QT_PLUGINS_DIR}\imageformats\qsvg.dll"

  ; ───────────────────────────────────────────────
  ; ALL translations → resources/qm/ only
  ; ───────────────────────────────────────────────
  SetOutPath "$INSTDIR\resources\qm"
  ; Qt standard translations
  File /nonfatal "${QT_TRANSLATIONS}\qt_*.qm"
  File /nonfatal "${QT_TRANSLATIONS}\qtbase_*.qm"
  ; LibreCAD / plugin translations (from build output or generated)
  File /nonfatal "..\..\windows\translations\*.qm"
  File /nonfatal "..\..\generated\Release\translations\*.qm"   ; adjust if your build puts them elsewhere

  ; Fonts (LFF)
  SetOutPath "$INSTDIR\resources\fonts"
  File /r "..\..\librecad\support\fonts\*.lff"

  ; Hatch patterns
  SetOutPath "$INSTDIR\resources\patterns"
  File /r "..\..\librecad\support\patterns\*.dxf"

  ; Library parts (with subfolders preserved)
  SetOutPath "$INSTDIR\resources\library"
  File /r "..\..\librecad\support\library\*.dxf"

  ; Registry & uninstaller
  WriteRegStr HKLM "Software\${APPNAME}" "" "$INSTDIR"
  WriteUninstaller "$INSTDIR\Uninstall.exe"

  ; Shortcuts (all users)
  SetShellVarContext all
  CreateDirectory "$SMPROGRAMS\${APPNAME}"
  CreateShortCut "$SMPROGRAMS\${APPNAME}\LibreCAD.lnk"        "$INSTDIR\LibreCAD.exe"
  CreateShortCut "$SMPROGRAMS\${APPNAME}\Uninstall.lnk"       "$INSTDIR\Uninstall.exe"
  CreateShortCut "$DESKTOP\LibreCAD.lnk"                      "$INSTDIR\LibreCAD.exe"

  ; Add/Remove Programs info
  WriteRegStr HKLM "${UNINSTKEY}" "DisplayName"     "${APPNAME} ${SCMREVISION} (${ARCH_SUFFIX})"
  WriteRegStr HKLM "${UNINSTKEY}" "DisplayIcon"     "$INSTDIR\LibreCAD.exe"
  WriteRegStr HKLM "${UNINSTKEY}" "DisplayVersion"  "${SCMREVISION}"
  WriteRegStr HKLM "${UNINSTKEY}" "Publisher"       "${COMPANY}"
  WriteRegStr HKLM "${UNINSTKEY}" "URLInfoAbout"    "https://librecad.org"
  WriteRegStr HKLM "${UNINSTKEY}" "InstallLocation" "$INSTDIR"
  WriteRegStr HKLM "${UNINSTKEY}" "UninstallString" '"$INSTDIR\Uninstall.exe"'
  WriteRegDWORD HKLM "${UNINSTKEY}" "NoModify" 1
  WriteRegDWORD HKLM "${UNINSTKEY}" "NoRepair" 1
SectionEnd

Section "Associate .dxf files" SecAssoc
  !ifdef AMD64
    SetRegView 64
  !else
    SetRegView 32
  !endif

  ReadRegStr $R0 HKCR ".dxf" ""
  ${If} $R0 != "LibreCAD.DXF"
    WriteRegStr HKLM "Software\${APPNAME}" "OldDXFAssoc" $R0
  ${EndIf}

  WriteRegStr HKCR ".dxf" "" "LibreCAD.DXF"
  WriteRegStr HKCR "LibreCAD.DXF" "" "DXF Drawing"
  WriteRegStr HKCR "LibreCAD.DXF\DefaultIcon" "" "$INSTDIR\LibreCAD.exe,0"
  WriteRegStr HKCR "LibreCAD.DXF\shell\open\command" "" '"$INSTDIR\LibreCAD.exe" "%1"'

  System::Call 'shell32::SHChangeNotify(i 0x08000000, i 0, i 0, i 0)'
SectionEnd

Section "Uninstall"
  !ifdef AMD64
    SetRegView 64
  !else
    SetRegView 32
  !endif

  SetShellVarContext all
  Delete "$DESKTOP\LibreCAD.lnk"
  RMDir /r "$SMPROGRAMS\${APPNAME}"
  RMDir /r "$INSTDIR"

  ; Restore original .dxf association if changed
  DeleteRegKey HKCR "LibreCAD.DXF"
  ReadRegStr $R0 HKCR ".dxf" ""
  ${If} $R0 == "LibreCAD.DXF"
    ReadRegStr $R1 HKLM "Software\${APPNAME}" "OldDXFAssoc"
    ${If} $R1 != ""
      WriteRegStr HKCR ".dxf" "" $R1
    ${Else}
      DeleteRegKey HKCR ".dxf"
    ${EndIf}
    System::Call 'shell32::SHChangeNotify(i 0x08000000, i 0, i 0, i 0)'
  ${EndIf}

  DeleteRegKey HKLM "Software\${APPNAME}"
  DeleteRegKey HKLM "${UNINSTKEY}"
SectionEnd

;───────────────────────────────────────────────
; Component descriptions
!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
  !insertmacro MUI_DESCRIPTION_TEXT ${SecMain}   "Core application files (required)."
  !insertmacro MUI_DESCRIPTION_TEXT ${SecAssoc}  "Set LibreCAD as default for .dxf files (double-click opens in LibreCAD)."
!insertmacro MUI_FUNCTION_DESCRIPTION_END

;───────────────────────────────────────────────
; Optional: auto-launch after install
Function LaunchLink
  ${If} ${SectionIsSelected} ${SecMain}
    ExecShell "" "$INSTDIR\LibreCAD.exe"
  ${EndIf}
FunctionEnd