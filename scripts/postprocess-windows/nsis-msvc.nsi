; nsis-msvc.nsi
; NSIS installer script for qmake + MSVC builds of LibreCAD (build-windows.bat)
; - Architecture-aware: x86, x64, ARM64 via /DAMD64 or /DARM64 from build-windows.bat
; - Per-arch registry keys identify which architecture owns the shared install
;   directory; installing another architecture replaces those registrations
; - Qt translations from Qt install dir + LibreCAD/plugin .qm from source tree
; - windeployqt output in windows\ is the primary file source
SetCompressor /SOLID lzma
!include "MUI2.nsh"
!include "LogicLib.nsh"
!include "WinVer.nsh"
;--------------------------------
; Allow overrides (Qt path, version, installer name, etc.)
!include /NONFATAL "custom.nsh"
;--------------------------------
; Version handling - priority:
; 1. /DSCMREVISION from command line (build-windows.bat)
; 2. generated_scmrev.nsh (if exists)
; 3. Default fallback
!ifdef SCMREVISION
    ; Already defined via command line - highest priority
!else
    !include /NONFATAL "generated_scmrev.nsh"
    !ifndef SCMREVISION
        !define SCMREVISION "2.2.x"
    !endif
!endif
;--------------------------------
; Basic definitions
!ifndef APPNAME
    !define APPNAME "LibreCAD"
!endif
!if "${APPNAME}" == "LibreCAD"
    !define OTHER_APPNAME "LibreCAD-beta"
!else
    !if "${APPNAME}" == "LibreCAD-beta"
        !define OTHER_APPNAME "LibreCAD"
    !else
        !error "APPNAME must be LibreCAD or LibreCAD-beta"
    !endif
!endif
!define MUI_ICON  "..\..\librecad\res\images\librecad.ico"
!define MUI_UNICON "..\..\desktop\res_old\main\uninstall.ico"
;--------------------------------
; Architecture suffix, install directory, and registry view
; /DAMD64 or /DARM64 passed by build-windows.bat; default is x86
!ifdef AMD64
    !define ARCH_SUFFIX "x64"
    !define INSTALL_DIR "$PROGRAMFILES64\${APPNAME}"
    !define REG_VIEW 64
!else
    !ifdef ARM64
        !define ARCH_SUFFIX "arm64"
        !define INSTALL_DIR "$PROGRAMFILES64\${APPNAME}"
        !define REG_VIEW 64
    !else
        !define ARCH_SUFFIX "x86"
        !define INSTALL_DIR "$PROGRAMFILES\${APPNAME}"
        !define REG_VIEW 32
    !endif
!endif
; Per-architecture registry paths identify the package currently installed.
!define UNINSTKEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}-${ARCH_SUFFIX}"
!define APPREG    "Software\${APPNAME}\${ARCH_SUFFIX}"
!define DXF_PROGID "${APPNAME}.DXF"
!define DWG_PROGID "${APPNAME}.DWG"
;--------------------------------
; General - Dynamic output filename with version and architecture
Name "${APPNAME} ${SCMREVISION}"
OutFile "../../generated/${APPNAME}-${SCMREVISION}-Windows-${ARCH_SUFFIX}.exe"
InstallDir "${INSTALL_DIR}"
InstallDirRegKey HKLM "${APPREG}" ""
RequestExecutionLevel admin
!ifndef VIProductVersion
    !define VIProductVersion "2.2.2.0"
!endif
VIProductVersion "${VIProductVersion}"
VIAddVersionKey "ProductName" "${APPNAME}"
VIAddVersionKey "FileVersion" "${SCMREVISION}"
VIAddVersionKey "ProductVersion" "${SCMREVISION}"
VIAddVersionKey "FileDescription" "${APPNAME} Installer"
VIAddVersionKey "LegalCopyright" "LibreCAD Team"
;--------------------------------
; Interface
!define MUI_ABORTWARNING
!define MUI_FINISHPAGE_RUN "$INSTDIR\LibreCAD.exe"
;--------------------------------
; Pages
!insertmacro MUI_PAGE_LICENSE "../../licenses/gpl-2.0.txt"
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_COMPONENTS
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES
;--------------------------------
; Languages (multilingual)
!insertmacro MUI_LANGUAGE "English"
!insertmacro MUI_LANGUAGE "French"
!insertmacro MUI_LANGUAGE "German"
!insertmacro MUI_LANGUAGE "Spanish"
!insertmacro MUI_LANGUAGE "SpanishInternational"
!insertmacro MUI_LANGUAGE "SimpChinese"
!insertmacro MUI_LANGUAGE "TradChinese"
!insertmacro MUI_LANGUAGE "Japanese"
!insertmacro MUI_LANGUAGE "Korean"
!insertmacro MUI_LANGUAGE "Italian"
!insertmacro MUI_LANGUAGE "Dutch"
!insertmacro MUI_LANGUAGE "Russian"
!insertmacro MUI_LANGUAGE "Polish"
!insertmacro MUI_LANGUAGE "Portuguese"
!insertmacro MUI_LANGUAGE "PortugueseBR"
!insertmacro MUI_LANGUAGE "Czech"
!insertmacro MUI_LANGUAGE "Swedish"
!insertmacro MUI_LANGUAGE "Finnish"
!insertmacro MUI_LANGUAGE "Greek"
!insertmacro MUI_LANGUAGE "Turkish"
!insertmacro MUI_LANGUAGE "Arabic"
;--------------------------------
; Qt paths (override in custom.nsh if needed)
!ifndef Qt_Dir
    !define Qt_Dir "C:\Qt"
!endif
!ifndef Qt_Version
    !define Qt_Version "5.15.2"
!endif
!ifndef Arch_Suffix
    !ifdef AMD64
        !define Arch_Suffix "_64"
    !else
        !ifdef ARM64
            !define Arch_Suffix "_arm64"
        !else
            !define Arch_Suffix ""
        !endif
    !endif
!endif
!ifndef MSVC_Ver
    !define MSVC_Ver "msvc2022${Arch_Suffix}"
!endif
!define QT_BIN_DIR "${Qt_Dir}\${Qt_Version}\${MSVC_Ver}\bin"
!define PLUGINS_DIR "${Qt_Dir}\${Qt_Version}\${MSVC_Ver}\plugins"
!define TRANSLATIONS_DIR "${Qt_Dir}\${Qt_Version}\${MSVC_Ver}\translations"
;--------------------------------
; Remove registrations only when their uninstall command is absent or stale.
; Active stable and beta channels may coexist. Architectures of the same channel
; share an install directory, so a new architecture replaces the old one.
!macro RemoveStaleRegistration PACKAGE_NAME ARCH VIEW
  SetRegView ${VIEW}
  ReadRegStr $R0 HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PACKAGE_NAME}-${ARCH}" "UninstallString"
  ReadRegStr $R1 HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PACKAGE_NAME}-${ARCH}" "InstallLocation"
  StrCpy $R2 "0"
  ${If} $R0 != ""
  ${AndIf} $R1 != ""
  ${AndIf} ${FileExists} "$R1\Uninstall.exe"
    StrCpy $R2 "1"
  ${EndIf}
  ${If} $R2 == "0"
  ${AndIf} $R0 != ""
  ${AndIf} ${FileExists} "$R0"
    StrCpy $R2 "1"
  ${EndIf}
  ${If} $R2 == "0"
    DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PACKAGE_NAME}-${ARCH}"
    DeleteRegKey HKLM "Software\${PACKAGE_NAME}\${ARCH}"
  ${EndIf}
  DeleteRegKey /ifempty HKLM "Software\${PACKAGE_NAME}"
!macroend

!macro RemoveStaleLegacyRegistration PACKAGE_NAME VIEW
  SetRegView ${VIEW}
  ReadRegStr $R0 HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PACKAGE_NAME}" "UninstallString"
  ReadRegStr $R1 HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PACKAGE_NAME}" "InstallLocation"
  StrCpy $R2 "0"
  ${If} $R0 != ""
  ${AndIf} $R1 != ""
  ${AndIf} ${FileExists} "$R1\Uninstall.exe"
    StrCpy $R2 "1"
  ${EndIf}
  ${If} $R2 == "0"
  ${AndIf} $R0 != ""
  ${AndIf} ${FileExists} "$R0"
    StrCpy $R2 "1"
  ${EndIf}
  ${If} $R2 == "0"
    DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PACKAGE_NAME}"
  ${EndIf}
!macroend

!macro MarkRegistrationActive PACKAGE_NAME ARCH VIEW
  SetRegView ${VIEW}
  ReadRegStr $R0 HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PACKAGE_NAME}-${ARCH}" "UninstallString"
  ReadRegStr $R1 HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PACKAGE_NAME}-${ARCH}" "InstallLocation"
  ${If} $R0 != ""
    ${If} $R1 != ""
    ${AndIf} ${FileExists} "$R1\Uninstall.exe"
      StrCpy $R3 "1"
    ${ElseIf} ${FileExists} "$R0"
      StrCpy $R3 "1"
    ${EndIf}
  ${EndIf}
!macroend

!macro MarkLegacyRegistrationActive PACKAGE_NAME VIEW
  SetRegView ${VIEW}
  ReadRegStr $R0 HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PACKAGE_NAME}" "UninstallString"
  ReadRegStr $R1 HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PACKAGE_NAME}" "InstallLocation"
  ${If} $R0 != ""
    ${If} $R1 != ""
    ${AndIf} ${FileExists} "$R1\Uninstall.exe"
      StrCpy $R3 "1"
    ${ElseIf} ${FileExists} "$R0"
      StrCpy $R3 "1"
    ${EndIf}
  ${EndIf}
!macroend

!macro RemoveOrphanedPackageAssociations PACKAGE_NAME
  ; A ProgID is shared by all architectures of a package channel. Remove it
  ; only after stale-key cleanup proves that no architecture is still active.
  StrCpy $R3 "0"
  !insertmacro MarkRegistrationActive "${PACKAGE_NAME}" "x64" 64
  !insertmacro MarkRegistrationActive "${PACKAGE_NAME}" "arm64" 64
  !insertmacro MarkRegistrationActive "${PACKAGE_NAME}" "x86" 32
  !insertmacro MarkLegacyRegistrationActive "${PACKAGE_NAME}" 64
  !insertmacro MarkLegacyRegistrationActive "${PACKAGE_NAME}" 32
  ${If} $R3 == "0"
    SetRegView 64
    ReadRegStr $R0 HKCR ".dxf" ""
    ${If} $R0 == "${PACKAGE_NAME}.DXF"
      DeleteRegValue HKCR ".dxf" ""
      DeleteRegKey /ifempty HKCR ".dxf"
    ${EndIf}
    ReadRegStr $R0 HKCR ".dwg" ""
    ${If} $R0 == "${PACKAGE_NAME}.DWG"
      DeleteRegValue HKCR ".dwg" ""
      DeleteRegKey /ifempty HKCR ".dwg"
    ${EndIf}
    DeleteRegKey HKCR "${PACKAGE_NAME}.DXF"
    DeleteRegKey HKCR "${PACKAGE_NAME}.DWG"
    SetRegView 32
    ReadRegStr $R0 HKCR ".dxf" ""
    ${If} $R0 == "${PACKAGE_NAME}.DXF"
      DeleteRegValue HKCR ".dxf" ""
      DeleteRegKey /ifempty HKCR ".dxf"
    ${EndIf}
    ReadRegStr $R0 HKCR ".dwg" ""
    ${If} $R0 == "${PACKAGE_NAME}.DWG"
      DeleteRegValue HKCR ".dwg" ""
      DeleteRegKey /ifempty HKCR ".dwg"
    ${EndIf}
    DeleteRegKey HKCR "${PACKAGE_NAME}.DXF"
    DeleteRegKey HKCR "${PACKAGE_NAME}.DWG"
  ${EndIf}
!macroend

!macro ReplaceArchitectureRegistration PACKAGE_NAME ARCH VIEW
  ; Carry the original association backup forward before removing the previous
  ; architecture's application key.
  SetRegView ${VIEW}
  ReadRegStr $R0 HKLM "Software\${PACKAGE_NAME}\${ARCH}" "OldDXFAssoc"
  ReadRegStr $R1 HKLM "Software\${PACKAGE_NAME}\${ARCH}" "OldDWGAssoc"
  SetRegView ${REG_VIEW}
  ReadRegStr $R2 HKLM "${APPREG}" "OldDXFAssoc"
  ${If} $R2 == ""
  ${AndIf} $R0 != ""
    WriteRegStr HKLM "${APPREG}" "OldDXFAssoc" $R0
  ${EndIf}
  ReadRegStr $R2 HKLM "${APPREG}" "OldDWGAssoc"
  ${If} $R2 == ""
  ${AndIf} $R1 != ""
    WriteRegStr HKLM "${APPREG}" "OldDWGAssoc" $R1
  ${EndIf}
  SetRegView ${VIEW}
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PACKAGE_NAME}-${ARCH}"
  DeleteRegKey HKLM "Software\${PACKAGE_NAME}\${ARCH}"
  DeleteRegKey /ifempty HKLM "Software\${PACKAGE_NAME}"
!macroend

;--------------------------------
; Installer Sections
Section "Main Section" SecMain
  SectionIn RO ; Required section
  ; The architectures share one install directory per package channel. Remove
  ; registrations for architectures replaced by this installer.
  !ifdef AMD64
    !insertmacro ReplaceArchitectureRegistration "${APPNAME}" "arm64" 64
    !insertmacro ReplaceArchitectureRegistration "${APPNAME}" "x86" 32
  !else
    !ifdef ARM64
      !insertmacro ReplaceArchitectureRegistration "${APPNAME}" "x64" 64
      !insertmacro ReplaceArchitectureRegistration "${APPNAME}" "x86" 32
    !else
      !insertmacro ReplaceArchitectureRegistration "${APPNAME}" "x64" 64
      !insertmacro ReplaceArchitectureRegistration "${APPNAME}" "arm64" 64
    !endif
  !endif
  ; The current channel supersedes its old unsuffixed registration.
  SetRegView 64
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}"
  SetRegView 32
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}"

  ; Stable and beta use different install directories. Preserve an active
  ; sibling channel, but remove its abandoned registrations.
  !insertmacro RemoveStaleRegistration "${OTHER_APPNAME}" "x64" 64
  !insertmacro RemoveStaleRegistration "${OTHER_APPNAME}" "arm64" 64
  !insertmacro RemoveStaleRegistration "${OTHER_APPNAME}" "x86" 32
  !insertmacro RemoveStaleLegacyRegistration "${OTHER_APPNAME}" 64
  !insertmacro RemoveStaleLegacyRegistration "${OTHER_APPNAME}" 32
  !insertmacro RemoveOrphanedPackageAssociations "${OTHER_APPNAME}"
  ; Set registry view based on architecture
  SetRegView ${REG_VIEW}
  ; Rebuild the current Add/Remove Programs key from known values below.
  DeleteRegKey HKLM "${UNINSTKEY}"
  SetOutPath "$INSTDIR"
  ; Copy all files from windeployqt output
  File /r /x "*.pdb" /x "translations" "..\..\windows\*.*"
  
  ; Ensure application icon is installed (required for shortcuts and Add/Remove Programs)
  File "${MUI_ICON}"
  
  ; Fallback Qt plugin copies (non-fatal; windeployqt should have handled these)
  SetOutPath "$INSTDIR\platforms"
  File /nonfatal "${PLUGINS_DIR}\platforms\qwindows.dll"
  File /nonfatal "${PLUGINS_DIR}\platforms\qminimal.dll"
  File /nonfatal "${PLUGINS_DIR}\platforms\qoffscreen.dll"
  SetOutPath "$INSTDIR\imageformats"
  File /nonfatal "${PLUGINS_DIR}\imageformats\qgif.dll"
  File /nonfatal "${PLUGINS_DIR}\imageformats\qico.dll"
  File /nonfatal "${PLUGINS_DIR}\imageformats\qjpeg.dll"
  File /nonfatal "${PLUGINS_DIR}\imageformats\qsvg.dll"
  File /nonfatal "${PLUGINS_DIR}\imageformats\qtiff.dll"
  SetOutPath "$INSTDIR\styles"
  File /nonfatal "${PLUGINS_DIR}\styles\qwindowsvistastyle.dll"
  ; Translations — all three sources into resources\qm (where rs_system searches)
  SetOutPath "$INSTDIR\resources\qm"
  File /nonfatal "${TRANSLATIONS_DIR}\qt_*.qm"
  File /nonfatal "${TRANSLATIONS_DIR}\qtbase_*.qm"
  File /nonfatal "..\..\librecad\ts\*.qm"
  File /nonfatal "..\..\plugins\ts\*.qm"
  ; === Package LFF fonts ===
  SetOutPath "$INSTDIR\resources\fonts"
  File /r "..\..\librecad\support\fonts\*.lff"
  ; === Package hatch patterns ===
  SetOutPath "$INSTDIR\resources\patterns"
  File /r "..\..\librecad\support\patterns\*.dxf"
  ; === Package library parts (DXF) - preserves subfolder structure ===
  SetOutPath "$INSTDIR\resources\library"
  File /r "..\..\librecad\support\library\*.dxf"
  ; Registry, shortcuts, uninstaller
  WriteRegStr HKLM "${APPREG}" "" "$INSTDIR"
  WriteUninstaller "$INSTDIR\Uninstall.exe"
  SetShellVarContext all
  CreateDirectory "$SMPROGRAMS\${APPNAME}"
  CreateShortCut "$SMPROGRAMS\${APPNAME}\${APPNAME}.lnk" "$INSTDIR\LibreCAD.exe" "" "$INSTDIR\librecad.ico"
  CreateShortCut "$SMPROGRAMS\${APPNAME}\Uninstall.lnk" "$INSTDIR\Uninstall.exe" "" "${MUI_UNICON}"
  CreateShortCut "$DESKTOP\${APPNAME}.lnk" "$INSTDIR\LibreCAD.exe" "" "$INSTDIR\librecad.ico"
  ; Add/Remove Programs entries - show architecture
  WriteRegStr HKLM "${UNINSTKEY}" "DisplayName" "${APPNAME} ${SCMREVISION} (${ARCH_SUFFIX})"
  WriteRegStr HKLM "${UNINSTKEY}" "DisplayIcon" "$INSTDIR\librecad.ico"
  WriteRegStr HKLM "${UNINSTKEY}" "DisplayVersion" "${SCMREVISION}"
  WriteRegStr HKLM "${UNINSTKEY}" "Publisher" "LibreCAD Team"
  WriteRegStr HKLM "${UNINSTKEY}" "URLInfoAbout" "https://librecad.org"
  WriteRegStr HKLM "${UNINSTKEY}" "InstallLocation" "$INSTDIR"
  WriteRegStr HKLM "${UNINSTKEY}" "UninstallString" '"$INSTDIR\Uninstall.exe"'
  WriteRegDWORD HKLM "${UNINSTKEY}" "NoModify" 1
  WriteRegDWORD HKLM "${UNINSTKEY}" "NoRepair" 1
  ; Check for errors on uninstaller registration
SectionEnd
Section "Associate .dxf and .dwg files" SecAssoc
  ; Set registry view (same as main section)
  SetRegView ${REG_VIEW}

  ; ── .dxf ──────────────────────────────────────────────────────────────────
  ; Back up existing association if not already ours
  ReadRegStr $R0 HKCR ".dxf" ""
  ${If} $R0 != "${DXF_PROGID}"
    WriteRegStr HKLM "${APPREG}" "OldDXFAssoc" $R0
  ${EndIf}
  ClearErrors
  WriteRegStr HKCR ".dxf"                         "" "${DXF_PROGID}"
  WriteRegStr HKCR "${DXF_PROGID}"                "" "AutoCAD DXF Drawing"
  WriteRegStr HKCR "${DXF_PROGID}\DefaultIcon"    "" "$INSTDIR\librecad.ico,0"
  WriteRegStr HKCR "${DXF_PROGID}\shell\open\command" "" '"$INSTDIR\LibreCAD.exe" "%1"'
  ${If} ${Errors}
    MessageBox MB_OK|MB_ICONEXCLAMATION "Failed to set .dxf file association!"
    Abort
  ${EndIf}

  ; ── .dwg ──────────────────────────────────────────────────────────────────
  ; Back up existing association if not already ours
  ReadRegStr $R0 HKCR ".dwg" ""
  ${If} $R0 != "${DWG_PROGID}"
    WriteRegStr HKLM "${APPREG}" "OldDWGAssoc" $R0
  ${EndIf}
  ClearErrors
  WriteRegStr HKCR ".dwg"                         "" "${DWG_PROGID}"
  WriteRegStr HKCR "${DWG_PROGID}"                "" "AutoCAD DWG Drawing"
  WriteRegStr HKCR "${DWG_PROGID}\DefaultIcon"    "" "$INSTDIR\librecad.ico,0"
  WriteRegStr HKCR "${DWG_PROGID}\shell\open\command" "" '"$INSTDIR\LibreCAD.exe" "%1"'
  ${If} ${Errors}
    MessageBox MB_OK|MB_ICONEXCLAMATION "Failed to set .dwg file association!"
    Abort
  ${EndIf}

  ; Notify Windows Shell of all association changes at once
  System::Call 'shell32::SHChangeNotify(i 0x08000000, i 0, i 0, i 0)'
SectionEnd
Section "Uninstall"
  ; Set registry view based on architecture
  SetRegView ${REG_VIEW}
  SetShellVarContext all
  Delete "$DESKTOP\${APPNAME}.lnk"
  RMDir /r "$SMPROGRAMS\${APPNAME}"
  RMDir /r "$INSTDIR"
  ; ── Restore .dxf association ────────────────────────────────────────────────
  DeleteRegKey HKCR "${DXF_PROGID}"
  ReadRegStr $R0 HKCR ".dxf" ""
  ${If} $R0 == "${DXF_PROGID}"
    ReadRegStr $R1 HKLM "${APPREG}" "OldDXFAssoc"
    ${If} $R1 == ""
      DeleteRegValue HKCR ".dxf" ""
      DeleteRegKey /ifempty HKCR ".dxf"
    ${Else}
      ReadRegStr $R2 HKCR "$R1\shell\open\command" ""
      ${If} $R2 == ""
        DeleteRegValue HKCR ".dxf" ""
        DeleteRegKey /ifempty HKCR ".dxf"
      ${Else}
        WriteRegStr HKCR ".dxf" "" $R1
      ${EndIf}
    ${EndIf}
  ${EndIf}

  ; ── Restore .dwg association ────────────────────────────────────────────────
  DeleteRegKey HKCR "${DWG_PROGID}"
  ReadRegStr $R0 HKCR ".dwg" ""
  ${If} $R0 == "${DWG_PROGID}"
    ReadRegStr $R1 HKLM "${APPREG}" "OldDWGAssoc"
    ${If} $R1 == ""
      DeleteRegValue HKCR ".dwg" ""
      DeleteRegKey /ifempty HKCR ".dwg"
    ${Else}
      ReadRegStr $R2 HKCR "$R1\shell\open\command" ""
      ${If} $R2 == ""
        DeleteRegValue HKCR ".dwg" ""
        DeleteRegKey /ifempty HKCR ".dwg"
      ${Else}
        WriteRegStr HKCR ".dwg" "" $R1
      ${EndIf}
    ${EndIf}
  ${EndIf}

  ; Notify Windows Shell of all association changes at once
  System::Call 'shell32::SHChangeNotify(i 0x08000000, i 0, i 0, i 0)'

  DeleteRegKey HKLM "${APPREG}"
  DeleteRegKey /ifempty HKLM "Software\${APPNAME}"
  DeleteRegKey HKLM "${UNINSTKEY}"
SectionEnd
;--------------------------------
; Component descriptions
!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
  !insertmacro MUI_DESCRIPTION_TEXT ${SecMain} "The core files required to run LibreCAD."
  !insertmacro MUI_DESCRIPTION_TEXT ${SecAssoc} "Associate .dxf and .dwg files with LibreCAD so double-clicking them opens in the application."
!insertmacro MUI_FUNCTION_DESCRIPTION_END
