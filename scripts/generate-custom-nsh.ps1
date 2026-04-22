# generate-custom-nsh.ps1
# Automatically generates custom.nsh for NSIS based on CI environment
# Usage: .\generate-custom-nsh.ps1 -Architecture AMD64|ARM64 -OutputPath "scripts\postprocess-windows\custom.nsh"

param(
    [Parameter(Mandatory=$true)]
    [ValidateSet("AMD64", "ARM64")]
    [string]$Architecture,
    
    [Parameter(Mandatory=$false)]
    [string]$OutputPath = "scripts\postprocess-windows\custom.nsh"
)

Write-Host "[INFO] Generating custom.nsh for $Architecture architecture..."

# Detect Qt installation path from environment variables
$qtDir = $null
$qtVersion = $null

# Try various environment variables that jurplel/install-qt-action might set
if ($env:Qt6_DIR) {
    $qtDir = $env:Qt6_DIR
    Write-Host "[INFO] Found Qt6_DIR: $qtDir"
} elseif ($env:QT_ROOT_DIR) {
    $qtDir = $env:QT_ROOT_DIR
    Write-Host "[INFO] Found QT_ROOT_DIR: $qtDir"
} elseif ($env:Qt5_DIR) {
    $qtDir = $env:Qt5_DIR
    Write-Host "[INFO] Found Qt5_DIR: $qtDir"
}

# If no env var found, try to detect from common locations
if (-not $qtDir) {
    Write-Host "[WARNING] No Qt environment variable found. Trying common locations..."
    
    # Check if Qt is installed in C:\Qt
    if (Test-Path "C:\Qt") {
        $qtDirs = Get-ChildItem "C:\Qt" -Directory | Where-Object { $_.Name -match '^\d+\.\d+' } | Sort-Object Name -Descending
        if ($qtDirs.Count -gt 0) {
            $qtDir = "C:\Qt"
            $qtVersion = $qtDirs[0].Name
            Write-Host "[INFO] Detected Qt at: $qtDir\$qtVersion"
        }
    }
    
    # GitHub Actions typical location
    if (-not $qtDir -and $env:GITHUB_WORKSPACE) {
        $githubQt = Join-Path $env:GITHUB_WORKSPACE "..\Qt"
        if (Test-Path $githubQt) {
            $qtDir = $githubQt
            Write-Host "[INFO] Using GitHub Actions Qt path: $qtDir"
        }
    }
}

# Fallback to default
if (-not $qtDir) {
    $qtDir = "C:\Qt"
    Write-Host "[WARNING] Using default Qt path: $qtDir"
}

# Extract Qt version from path if not already set
if (-not $qtVersion) {
    # Try to extract version from path like "C:\Qt\6.9.0" or "D:\a\LibreCAD\Qt\6.9.0"
    if ($qtDir -match '\\(\d+\.\d+\.\d+)\\?$') {
        $qtVersion = $Matches[1]
    } else {
        # Look for version subdirectory
        $versionDirs = Get-ChildItem $qtDir -Directory | Where-Object { $_.Name -match '^\d+\.\d+' } | Sort-Object Name -Descending
        if ($versionDirs.Count -gt 0) {
            $qtVersion = $versionDirs[0].Name
        } else {
            $qtVersion = "6.9.0"
            Write-Host "[WARNING] Could not detect Qt version, using default: $qtVersion"
        }
    }
}

Write-Host "[INFO] Qt Directory: $qtDir"
Write-Host "[INFO] Qt Version: $qtVersion"

# Determine MSVC variant based on architecture
if ($Architecture -eq "AMD64") {
    $msvcVer = "msvc2022_64"
    $archSuffix = "_64"
    $appKeyName = "LibreCADx64"
    $appName = "LibreCAD (x64)"
    $programsFolder = '$PROGRAMFILES64'
} elseif ($Architecture -eq "ARM64") {
    $msvcVer = "msvc2022_arm64"
    $archSuffix = "_arm64"
    $appKeyName = "LibreCADARM64"
    $appName = "LibreCAD (ARM64)"
    $programsFolder = '$PROGRAMFILES64'
    # Note: ARM64 is already defined via /DARM64 command-line parameter in build-nsis.bat
}

Write-Host "[INFO] MSVC Variant: $msvcVer"
Write-Host "[INFO] Application Name: $appName"

# Generate custom.nsh content
$nshContent = "; Auto-generated custom.nsh for NSIS installer`n"
$nshContent += "; Generated on: $(Get-Date -Format 'yyyy-MM-dd HH:mm:ss')`n"
$nshContent += "; Architecture: $Architecture`n"
$nshContent += "`n"
$nshContent += "!define Qt_Dir `"$qtDir`"`n"
$nshContent += "!define Qt6_Dir `"$qtDir`"`n"
$nshContent += "!define Qt_Version `"$qtVersion`"`n"
$nshContent += "!define Mingw_Ver `"$msvcVer`"`n"
$nshContent += "!define InstallerName `"LibreCAD-Installer`"`n"
$nshContent += "!define ProgramsFolder `"$programsFolder`"`n"
$nshContent += "!define AppKeyName `"$appKeyName`"`n"
$nshContent += "!define AppName `"$appName`"`n"

# Ensure output directory exists
$outputDir = Split-Path $OutputPath -Parent
if (-not (Test-Path $outputDir)) {
    New-Item -ItemType Directory -Force -Path $outputDir | Out-Null
    Write-Host "[INFO] Created directory: $outputDir"
}

# Write to file
try {
    $nshContent | Out-File -FilePath $OutputPath -Encoding UTF8 -NoNewline
    Write-Host "[SUCCESS] custom.nsh generated successfully at: $OutputPath"
    Write-Host "[INFO] Content preview:"
    Write-Host $nshContent
} catch {
    $errorMsg = "[ERROR] Failed to write custom.nsh: " + $_.Exception.Message
    Write-Error $errorMsg
    exit 1
}
