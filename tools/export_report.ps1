# export_report.ps1 - run gradebook-cli and export the report as CSV.
#
# Usage:
#   powershell -File tools/export_report.ps1 -Data data/scores.csv -Out report.csv
#
# NOTE: ASCII-only on purpose (see build.ps1).
[CmdletBinding()]
param(
    [string]$Data = 'data/scores.csv',
    [string]$Out = 'report.csv',
    [string]$Cli = 'build/gradebook-cli.exe'
)

$ErrorActionPreference = 'Stop'
if (-not (Test-Path $Cli)) { throw "executable not found: $Cli (run build.ps1 first)" }

$lines = & $Cli $Data
$rows = New-Object System.Collections.Generic.List[string]
$rows.Add('name,average,max')
foreach ($line in $lines) {
    if ($line -match '^(.+?):\s+average=([0-9.]+),\s+max=([0-9]+)\s*$') {
        $rows.Add(('{0},{1},{2}' -f $Matches[1], $Matches[2], $Matches[3]))
    }
}

$outPath = $Out
if (-not [System.IO.Path]::IsPathRooted($outPath)) {
    $outPath = Join-Path (Get-Location).Path $outPath
}
[System.IO.File]::WriteAllLines($outPath, $rows)
Write-Host ("wrote " + $outPath + " with " + ($rows.Count - 1) + " record(s)")
