# Copyright (C) 2026 librecad.org and Dongxu Li
$ErrorActionPreference = 'Stop'
$allowed = @('.github/workflows/build-all.yml', 'scripts/postprocess-windows/nsis-msvc.nsi')
$changed = @(git diff --name-only 193a366d14821efa1b6279522de8b5d287f3ce45 HEAD)
if ($LASTEXITCODE -ne 0 -or @($changed | Where-Object { $_ -notin $allowed }).Count -ne 0) {
    throw 'Application source changed; prebuilt binaries cannot validate this PR.'
}
New-Item -ItemType Directory -Force generated | Out-Null
Copy-Item prebuilt/generated/Release generated/Release -Recurse
Set-Content generated/Release/librecad_lib.lib 'Build-only archive must not be installed.'
Set-Content generated/Release/application.pdb 'Debug symbols must not be installed.'
New-Item -ItemType Directory -Force generated/Release/sdk | Out-Null
Set-Content generated/Release/sdk/nested.lib 'Nested build-only archive must not be installed.'
$old = @(Get-ChildItem prebuilt/generated -Filter LibreCAD-*.exe -File)
if ($old.Count -ne 1) { throw 'Expected exactly one verified source installer.' }
$extracted = Join-Path $env:RUNNER_TEMP 'verified-previous-payload'
& 7z x -y "-o$extracted" $old[0].FullName > $null
if ($LASTEXITCODE -ne 0) { throw 'Could not extract previously verified translations.' }
Copy-Item "$extracted/resources/qm/librecad_*.qm" librecad/ts
Copy-Item "$extracted/resources/qm/plugins_*.qm" plugins/ts
Write-Host 'Verified: the PR changes only packaging, so the native application code is identical.'
