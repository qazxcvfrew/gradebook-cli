# build.ps1 - Windows build script: tries g++, falls back to MSVC cl.exe.
# Usage: powershell -File build.ps1 [-Test]
#
# g++ is only accepted if it actually produces a binary: on some locked-down
# machines (and inside restricted sandboxes) the MSYS2 driver cannot spawn its
# own cc1plus child, in which case the build silently must fall back to MSVC.
#
# NOTE: ASCII-only on purpose. Windows PowerShell 5.1 reads BOM-less files with
# the system ANSI code page, which would mangle non-ASCII text.
[CmdletBinding()]
param([switch]$Test)

$ErrorActionPreference = 'Stop'
$root = $PSScriptRoot
$build = Join-Path $root 'build'
New-Item -ItemType Directory -Force -Path $build | Out-Null

$srcDir = Join-Path $root 'src'
$cliSources = @((Join-Path $srcDir 'main.cpp'), (Join-Path $srcDir 'gradebook.cpp'))
$testSources = @((Join-Path $root 'tests\test_gradebook.cpp'), (Join-Path $srcDir 'gradebook.cpp'))

function Resolve-Gpp {
    foreach ($candidate in @('C:\msys64\ucrt64\bin\g++.exe', 'C:\msys64\mingw64\bin\g++.exe')) {
        if (Test-Path $candidate) { return $candidate }
    }
    $cmd = Get-Command g++ -ErrorAction SilentlyContinue
    if ($cmd) { return $cmd.Source }
    return $null
}

$targets = @()
if ($Test) {
    $targets += @{ Name = 'tests'; Sources = $testSources }
} else {
    $targets += @{ Name = 'gradebook-cli'; Sources = $cliSources }
    $targets += @{ Name = 'tests'; Sources = $testSources }
}

$gpp = Resolve-Gpp
if ($gpp) {
    $previous = $ErrorActionPreference
    $ErrorActionPreference = 'Continue'
    $gppOk = $true
    foreach ($target in $targets) {
        $out = Join-Path $build ($target.Name + '.exe')
        if (Test-Path $out) { Remove-Item $out -Force }
        & $gpp -std=c++17 -Wall -Wextra -O2 @($target.Sources) -o $out 2>&1 | Out-Null
        if ($LASTEXITCODE -ne 0 -or -not (Test-Path $out)) { $gppOk = $false; break }
    }
    $ErrorActionPreference = $previous
    if ($gppOk) {
        Write-Host "[build] compiler: $gpp"
        foreach ($target in $targets) { Write-Host ('[build] ok: ' + (Join-Path $build ($target.Name + '.exe'))) }
        exit 0
    }
    Write-Host "[build] g++ present but unusable ($gpp) - falling back to MSVC"
}

$vcvars = 'C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat'
if (Test-Path $vcvars) {
    Write-Host '[build] compiler: cl.exe (MSVC)'
    foreach ($target in $targets) {
        $out = Join-Path $build ($target.Name + '.exe')
        $quoted = ($target.Sources | ForEach-Object { '"' + $_ + '"' }) -join ' '
        $inner = '"' + $vcvars + '" >nul 2>&1 && cl /nologo /std:c++17 /EHsc /utf-8 /Fe:"' + $out + '" ' + $quoted
        cmd /c $inner
        if ($LASTEXITCODE -ne 0 -or -not (Test-Path $out)) { throw "cl failed for $($target.Name)" }
        Write-Host "[build] ok: $out"
    }
    exit 0
}

throw 'no usable C++ compiler found (tried g++ and MSVC cl.exe)'
