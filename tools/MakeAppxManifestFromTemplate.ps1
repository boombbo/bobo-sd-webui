﻿# Make a valid AppxManifest.xml from a templated file and variable substititon

Param(
    [Parameter(Mandatory)]
    [ValidateNotNullOrEmpty()]
    [string]$TemplateFile,

    [Parameter(Mandatory)]
    [ValidateNotNullOrEmpty()]
    [string]$ManifestFile,

    [Parameter(Mandatory)]
    [ValidateSet("arm", "arm64", "neutral", "x64", "x86")]
    [string]$Architecture,

    [string]$Version,

    [string]$VersionMajor,
    [string]$VersionMinor
)

Write-Output "TemplateFile $TemplateFile"
Write-Output "ManifestFile $ManifestFile"
Write-Output "Architecture $Architecture"

if (-not ([string]::IsNullOrEmpty($Version)))
{
    if ($Version -NotMatch "^[0-9]{1,5}\.[0-9]{1,5}\.[0-9]{1,5}\.[0-9]{1,5}$")
    {
        Write-Output "Invalid parameter -Version $Version, aborting..."
        Exit 1
    }
    $DotQuadVersion = $Version.Split(".")
    $Major = DotQuadVersion[0]

    if (-not ([string]::IsNullOrEmpty($VersionMajor)))
    {
        if ($Major -ne $VersionMajor)
        {
            Write-Output "Invalid parameter -Version $Version != -VersionMajor $VersionMajor, aborting..."
            Exit 1
        }
    }
    $VersionMajor = $Major
}

Write-Output "Reading $TemplateFile..."
$manifest = [IO.File]::ReadAllText("$TemplateFile")
$original = $manifest

Write-Output "Transforming $TemplateFile..."
if (-not ([string]::IsNullOrEmpty($Version)))
{
    $manifest = "$manifest" -replace "###version###", "$Version"
}
if (-not ([string]::IsNullOrEmpty($VersionMajor)))
{
    $manifest = "$manifest" -replace "###version.major###", "$Version_Major"
}
if (-not ([string]::IsNullOrEmpty($VersionMinor)))
{
    $manifest = "$manifest" -replace "###version.minor###", "$Version_Minor"
}
$manifest = "$manifest" -replace "###architecture###", "$Architecture"
if ($Architecture -eq "x64")
{
    $ShortArchitecture = "x6"
}
elseif ($Architecture -eq "x86")
{
    $ShortArchitecture = "x8"
}
elseif ($Architecture -eq "arm64")
{
    $ShortArchitecture = "a6"
}
if (-not ([string]::IsNullOrEmpty($ShortArchitecture)))
{
    $manifest = "$manifest" -replace "###shortarchitecture###", "$ShortArchitecture"
}

if ($manifest -ne $original)
{
    Write-Output "Writing $ManifestFile..."
    "$manifest" | Out-File "$ManifestFile" -Encoding utf8
}
else
{
    Write-Output "$ManifestFile unchanged"
}
