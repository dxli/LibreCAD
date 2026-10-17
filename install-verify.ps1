# Copyright (C) 2026 librecad.org and Dongxu Li
$ErrorActionPreference = 'Stop'
$evidence = Join-Path (Get-Location).Path 'install-evidence'
New-Item -ItemType Directory -Force $evidence | Out-Null
$report = Join-Path $evidence 'installation.txt'

function Assert-True($condition, [string]$message) {
    if (-not $condition) { throw $message }
    $message | Tee-Object -FilePath $report -Append | Write-Host
}

Assert-True ([Runtime.InteropServices.RuntimeInformation]::OSArchitecture.ToString() -eq 'Arm64') 'Test runner is native Windows ARM64.'
Assert-True (-not $env:QT_ROOT_DIR) 'No development Qt root is configured.'
Assert-True (-not (Get-Command windeployqt6.exe -ErrorAction SilentlyContinue)) 'No development windeployqt is available.'
Assert-True (-not (Test-Path (Join-Path $env:GITHUB_WORKSPACE '../Qt'))) 'The build runner Qt installation does not exist on this runner.'
$installers = @(Get-ChildItem build-artifact/generated -Filter LibreCAD-*.exe -File)
Assert-True ($installers.Count -eq 1) 'Exactly one tested installer was downloaded.'
$installer = $installers[0]
Get-FileHash $installer.FullName -Algorithm SHA256 | Format-List | Out-File "$evidence/installer-sha256.txt"
$destination = Join-Path $env:RUNNER_TEMP 'LibreCAD-pr2876-installed'

function Wait-Native($process, [string]$name, [int]$timeout = 60000) {
    if (-not $process.WaitForExit($timeout)) {
        $process.Kill()
        throw "Process timed out: $name"
    }
    $process.Refresh()
    Assert-True ($process.ExitCode -eq 0) "Process exits successfully: $name"
}

$process = Start-Process -FilePath $installer.FullName -ArgumentList @('/S', "/D=$destination") -PassThru
Wait-Native $process 'silent installation' 180000
$app = Join-Path $destination 'LibreCAD.exe'
Assert-True (Test-Path $app) 'Silent installation installs LibreCAD.exe.'
Assert-True (Test-Path "$destination/Qt6Core.dll") 'Silent installation installs the Qt runtime.'
Assert-True (Test-Path "$destination/vcruntime140.dll") 'Silent installation installs the app-local Visual C++ runtime.'
$uninstallKey = 'HKLM:/Software/Microsoft/Windows/CurrentVersion/Uninstall/LibreCAD-arm64'
Assert-True (Test-Path $uninstallKey) 'Installer registers its ARM64 uninstall entry.'
$metadata = Get-ItemProperty $uninstallKey
$metadata | Format-List | Out-File "$evidence/uninstall-metadata.txt"
Assert-True ($metadata.InstallLocation -eq $destination) 'Registered install location is correct.'
$shortcut = Join-Path ([Environment]::GetFolderPath('CommonPrograms')) 'LibreCAD (ARM64)/LibreCAD (ARM64).lnk'
Assert-True (Test-Path $shortcut) 'Installer creates the Start-menu shortcut.'
$shell = New-Object -ComObject WScript.Shell
$target = $shell.CreateShortcut($shortcut).TargetPath
Assert-True ($target -eq $app) 'Start-menu shortcut points to the installed application.'

$env:QT_QPA_PLATFORM = 'offscreen'
function Run-App([string]$name, [string[]]$arguments) {
    $process = Start-Process -FilePath $app -ArgumentList $arguments -WorkingDirectory $destination -PassThru -RedirectStandardOutput "$evidence/$name.stdout.txt" -RedirectStandardError "$evidence/$name.stderr.txt"
    Wait-Native $process $name
}
Run-App 'version' @('dxf2svg', '--version')
$version = (Get-Content "$evidence/version.stdout.txt" -Raw) + (Get-Content "$evidence/version.stderr.txt" -Raw)
Assert-True ($version -match 'LibreCAD') 'Installed application reports its version without development Qt.'
$dxf = "0`nSECTION`n2`nHEADER`n9`n`$ACADVER`n1`nAC1015`n0`nENDSEC`n0`nSECTION`n2`nENTITIES`n0`nLINE`n8`n0`n10`n0`n20`n0`n30`n0`n11`n100`n21`n100`n31`n0`n0`nENDSEC`n0`nEOF`n"
Set-Content "$destination/smoke.dxf" $dxf -Encoding ascii
Run-App 'dxf2svg' @('dxf2svg', '--resolution', '512x512', '--outfile', 'smoke.svg', 'smoke.dxf')
$svg = Join-Path $destination 'smoke.svg'
Assert-True ((Test-Path $svg) -and (Get-Item $svg).Length -gt 100) 'Installed application converts a DXF to SVG without development Qt.'
[xml]$document = Get-Content $svg -Raw
Assert-True ($document.DocumentElement.LocalName -eq 'svg') 'Installed application produces valid SVG XML.'
Copy-Item $svg $evidence

$env:QT_QPA_PLATFORM = 'windows'
$gui = Start-Process -FilePath $app -WorkingDirectory $destination -PassThru
try {
    Start-Sleep -Seconds 10
    $gui.Refresh()
    Assert-True (-not $gui.HasExited) 'Installed GUI remains running with the native Windows platform plugin.'
    Assert-True ($gui.MainWindowHandle -ne 0) 'Installed GUI creates a native application window.'
    "Window title: $($gui.MainWindowTitle)" | Out-File "$evidence/gui-window.txt"
} finally {
    if (-not $gui.HasExited) {
        $null = $gui.CloseMainWindow()
        if (-not $gui.WaitForExit(5000)) { $gui.Kill(); $gui.WaitForExit() }
    }
}
$files = @(Get-ChildItem $destination -File -Recurse)
$files | ForEach-Object { $_.FullName.Substring($destination.Length + 1) } | Out-File "$evidence/installed-files.txt"
$files | ForEach-Object { [pscustomobject]@{Name = $_.FullName.Substring($destination.Length + 1); Bytes = $_.Length} } | ConvertTo-Json | Out-File "$evidence/installed-sizes.json"
$process = Start-Process -FilePath "$destination/Uninstall.exe" -ArgumentList @('/S', "_?=$destination") -PassThru
Wait-Native $process 'silent uninstallation' 180000
Assert-True (-not (Test-Path $uninstallKey)) 'Uninstaller removes the ARM64 uninstall entry.'
exit 0
