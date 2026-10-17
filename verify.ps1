# Copyright (C) 2026 librecad.org and Dongxu Li
param([ValidateSet('Failures', 'Payload')][string]$Mode)
$ErrorActionPreference = 'Stop'
$root = (Get-Location).Path
$evidence = Join-Path $root 'pr2876-evidence'
New-Item -ItemType Directory -Force $evidence | Out-Null
$report = Join-Path $evidence "$Mode.txt"

function Assert-True($condition, [string]$message) {
    if (-not $condition) { throw $message }
    $message | Tee-Object -FilePath $report -Append | Write-Host
}

if ($Mode -eq 'Failures') {
    $sandbox = Join-Path $env:RUNNER_TEMP 'pr2876-deployment-tests'
    New-Item -ItemType Directory -Force $sandbox | Out-Null
    foreach ($case in @('old-path', 'missing-exe', 'copy-failure', 'success')) {
        $directory = Join-Path $sandbox $case
        New-Item -ItemType Directory -Force "$directory/generated/ARM64/Release" | Out-Null
        Set-Content "$directory/generated/ARM64/Release/intermediate.tlog" 'not an application'
        if ($case -in @('copy-failure', 'success')) {
            Copy-Item "$root/generated/Release" "$directory/generated/Release" -Recurse
        }
        if ($case -eq 'copy-failure') {
            Set-Content "$directory/windows" 'a file, not a staging directory'
        }
        $batch = if ($case -eq 'old-path') { 'original-deploy.cmd' } else { 'fixed-deploy.cmd' }
        Copy-Item "$PSScriptRoot/$batch" "$directory/deploy.cmd"
        Push-Location $directory
        try {
            & cmd.exe /d /c deploy.cmd 2>&1 | Tee-Object -FilePath "$evidence/$case.log" | Write-Host
            $code = $LASTEXITCODE
        } finally {
            Pop-Location
        }
        $reached = Test-Path "$directory/reached.txt"
        switch ($case) {
            'old-path' {
                Assert-True ($code -eq 0 -and $reached) 'Baseline reproduces silent success after failed windeployqt.'
                Assert-True (-not (Test-Path "$directory/windows/LibreCAD.exe")) 'Baseline stages no application.'
                Assert-True (Test-Path "$directory/windows/intermediate.tlog") 'Baseline stages build intermediates instead.'
            }
            'missing-exe' {
                Assert-True ($code -ne 0 -and -not $reached) 'Fixed batch stops on real windeployqt failure.'
                Assert-True (-not (Test-Path "$directory/windows")) 'No staging occurs after failed deployment.'
            }
            'copy-failure' {
                Assert-True (Test-Path "$directory/generated/Release/Qt6Core.dll") 'windeployqt succeeded before the injected copy failure.'
                Assert-True ($code -ne 0 -and -not $reached) 'Fixed batch stops on real xcopy failure.'
            }
            'success' {
                Assert-True ($code -eq 0 -and $reached) 'Fixed batch succeeds for valid deployment.'
                Assert-True (Test-Path "$directory/windows/LibreCAD.exe") 'Fixed batch stages the application.'
                Assert-True (Test-Path "$directory/windows/Qt6Core.dll") 'Fixed batch stages the Qt runtime.'
            }
        }
    }
    exit 0
}

$installers = @(Get-ChildItem "$root/generated/LibreCAD-*.exe" -File)
Assert-True ($installers.Count -eq 1) 'Exactly one NSIS installer was produced.'
$installer = $installers[0]
"Installer: $($installer.Name), $($installer.Length) bytes" | Tee-Object -FilePath $report -Append | Write-Host
$extracted = Join-Path $env:RUNNER_TEMP 'pr2876-extracted-installer'
& 7z x -y "-o$extracted" $installer.FullName > "$evidence/extract.log"
Assert-True ($LASTEXITCODE -eq 0) '7-Zip extracts the installer successfully.'
$applications = @(Get-ChildItem $extracted -Filter LibreCAD.exe -File -Recurse)
Assert-True ($applications.Count -eq 1) 'Installer contains exactly one LibreCAD.exe.'
$app = $applications[0]
$appDir = $app.DirectoryName
Assert-True ((Get-FileHash $app.FullName).Hash -eq (Get-FileHash "$root/windows/LibreCAD.exe").Hash) 'Packaged application matches the staged application byte-for-byte.'

function Get-PeMachine([string]$path) {
    $stream = [IO.File]::OpenRead($path)
    $reader = [IO.BinaryReader]::new($stream)
    try {
        Assert-True ($reader.ReadUInt16() -eq 0x5a4d) "Valid DOS header: $([IO.Path]::GetFileName($path))"
        $stream.Position = 0x3c
        $offset = $reader.ReadInt32()
        $stream.Position = $offset
        Assert-True ($reader.ReadUInt32() -eq 0x4550) "Valid PE header: $([IO.Path]::GetFileName($path))"
        return $reader.ReadUInt16()
    } finally { $reader.Dispose() }
}

$required = @('LibreCAD.exe', 'Qt6Core.dll', 'Qt6Gui.dll', 'Qt6Widgets.dll',
              'Qt6Network.dll', 'Qt6PrintSupport.dll', 'Qt6Svg.dll',
              'platforms/qwindows.dll', 'platforms/qoffscreen.dll', 'imageformats/qsvg.dll')
foreach ($relative in $required) {
    $path = Join-Path $appDir $relative
    Assert-True (Test-Path $path) "Required payload exists: $relative"
    Assert-True ((Get-PeMachine $path) -eq 0xaa64) "Payload is native ARM64: $relative"
}
foreach ($runtime in @('msvcp140.dll', 'vcruntime140.dll')) {
    $path = Join-Path $appDir $runtime
    Assert-True (Test-Path $path) "App-local runtime exists: $runtime"
    Assert-True ((Get-PeMachine $path) -in @(0xaa64, 0xa641, 0xa64e)) "Runtime is ARM64-compatible: $runtime"
}
Assert-True (@(Get-ChildItem "$appDir/resources/fonts" -Filter *.lff -Recurse).Count -gt 0) 'Installer contains CAD fonts.'
Assert-True (@(Get-ChildItem "$appDir/resources/qm" -Filter librecad_*.qm).Count -gt 0) 'Installer contains application translations.'
Assert-True (@(Get-ChildItem $appDir -Recurse -Filter *.tlog).Count -eq 0) 'Installer contains no MSBuild tlog intermediates.'

$env:QT_QPA_PLATFORM = 'offscreen'
$env:PATH = (($env:PATH -split ';') | Where-Object { $_ -notmatch '(?i)[\\/]Qt[\\/]|msvc2022|[\\/]VC[\\/]' }) -join ';'
Remove-Item Env:QT_PLUGIN_PATH -ErrorAction SilentlyContinue
Remove-Item Env:QT_QPA_PLATFORM_PLUGIN_PATH -ErrorAction SilentlyContinue
Remove-Item Env:QT_ROOT_DIR -ErrorAction SilentlyContinue
Remove-Item Env:Qt6_DIR -ErrorAction SilentlyContinue

function Run-App([string]$name, [string[]]$arguments) {
    $process = Start-Process -FilePath $app.FullName -ArgumentList $arguments -WorkingDirectory $appDir -PassThru -RedirectStandardOutput "$evidence/$name.stdout.txt" -RedirectStandardError "$evidence/$name.stderr.txt"
    if (-not $process.WaitForExit(60000)) {
        $process.Kill()
        throw "Application timed out: $name"
    }
    $process.Refresh()
    Assert-True ($process.ExitCode -eq 0) "Packaged application exits successfully: $name"
}
Run-App 'version' @('dxf2svg', '--version')
$version = (Get-Content "$evidence/version.stdout.txt" -Raw) + (Get-Content "$evidence/version.stderr.txt" -Raw)
Assert-True ($version -match 'LibreCAD') 'Packaged application reports its version.'
$dxf = "0`nSECTION`n2`nHEADER`n9`n`$ACADVER`n1`nAC1015`n0`nENDSEC`n0`nSECTION`n2`nENTITIES`n0`nLINE`n8`n0`n10`n0`n20`n0`n30`n0`n11`n100`n21`n100`n31`n0`n0`nENDSEC`n0`nEOF`n"
Set-Content -Path "$appDir/pr2876-smoke.dxf" -Value $dxf -Encoding ascii
Run-App 'dxf2svg' @('dxf2svg', '--resolution', '512x512', '--outfile', 'pr2876-smoke.svg', 'pr2876-smoke.dxf')
$svg = Join-Path $appDir 'pr2876-smoke.svg'
Assert-True ((Test-Path $svg) -and (Get-Item $svg).Length -gt 100) 'Packaged application converts a DXF to a nonempty SVG without Qt on PATH.'
[xml]$document = Get-Content $svg -Raw
Assert-True ($document.DocumentElement.LocalName -eq 'svg') 'Converted output is valid SVG XML.'
Copy-Item $svg $evidence
Get-FileHash $installer.FullName -Algorithm SHA256 | Format-List | Out-File "$evidence/installer-sha256.txt"
Get-ChildItem $extracted -File -Recurse | ForEach-Object { $_.FullName.Substring($extracted.Length + 1) } | Out-File "$evidence/payload-files.txt"
exit 0
