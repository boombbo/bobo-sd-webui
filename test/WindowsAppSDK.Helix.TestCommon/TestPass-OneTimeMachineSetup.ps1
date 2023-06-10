Write-Host "TestPass-OneTimeMachineSetup.ps1"

# This script is run once on a test machine.
# It is a good place to add any logic to install test dependencies, etc.

# This script is called by TestPass-OneTimeMachineSetupCore.ps1 which is a part of the WinUI.Helix package.

# Install and start the TAEF Service on the machine.
# Functions copied and modified from https://github.com/microsoft/WindowsAppSDK/blob/main/tools/DevCheck.ps1
function Test-TAEFService
{
    $service = Get-Service |Where-Object {$_.Name -eq "TE.Service"}
    if ([string]::IsNullOrEmpty($service))
    {
        Write-Host "TAEF service...Not Installed"
        return 'NotFound'
    }
    elseif ($service.Status -ne "Running")
    {
        Write-Host "TAEF service...Not running ($service.Status)"
        return 'NotRunning'
    }
    else
    {
        Write-Host "TAEF service...Running"
        return 'Running'
    }
}
function Repair-TAEFService
{
    $path = ".\Wex.Services.exe"
    if (-not(Test-Path -Path $path -PathType Leaf))
    {
        Write-Host "Install TAEF service...Not Found ($path)"
        return 'TAEFNotFound'
    }
    $args = '/install:TE.Service'
    & $path $args
    $service = Get-Service |Where-Object {$_.Name -eq "TE.Service"}
    if ([string]::IsNullOrEmpty($service))
    {
        Write-Host "Install TAEF service...Failed"
        return 'InstallError'
    }
    else
    {
        Write-Host "Install TAEF service...OK"
        return 'NotRunning'
    }
}
function Start-TAEFService
{
    $ok = Start-Service 'TE.Service'
    $service = Get-Service |Where-Object {$_.Name -eq "TE.Service"}
    if ($service.Status -ne "Running")
    {
        Write-Host "Start TAEF service...Failed"
    }
    else
    {
        Write-Host "Start TAEF service...OK"
        return $true
    }
}

$test = Test-TAEFService
if ($test -eq 'NotFound')
{
    $test = Repair-TAEFService
}
if ($test -eq 'NotRunning')
{
    $test = Start-TAEFService
}

$packagesToFind = @(
    "Microsoft.VCLibs.*.appx",
    "Microsoft.NET.CoreRuntime.*.appx",
    "Microsoft.NET.CoreFramework.*.appx",
    "Microsoft.NET.Native.Framework.*.appx",
    "Microsoft.NET.Native.Runtime.*.appx"
)

foreach ($pattern in $packagesToFind)
{
    foreach ($package in (Get-ChildItem $pattern))
    {
        Write-Host "Installing $package"
        Add-AppxPackage $package -ErrorVariable appxerror -ErrorAction SilentlyContinue
        if ($appxerror)
        {
            foreach ($error in $appxerror)
            {
                # In the case where the package does not install becasuse a higher version is already installed
                # we don't want to print an error message, since that is just noise. Filter out such errors.
                if (($error.Exception.Message -match "0x80073D06") -or ($error.Exception.Message -match "0x80073CFB"))
                {
                    Write-Host "The same or higher version of this package is already installed."
                }
                else
                {
                    Write-Error $error
                }
            }
        }
    }
}

# Install any certificates (*.cer) included in the "certificates" folder from the BuildOutput.
# NOTE: When building up the "testPayloadDir" in WindowsAppSDK-RunHelixTests-Job.yml, the BuildOutput pipeline artifact is
# unpacked under: $(Build.SourcesDirectory)\BuildOutput\$(buildConfiguration)\$(buildPlatform)\HelixTests
# However, it retains the same folder structure as within the artifact when unpacked, so the resulting folder structure looks like:
#  $(Build.SourcesDirectory)\BuildOutput\$(buildConfiguration)\$(buildPlatform)\HelixTests\BuildOutput\$(buildConfiguration)\$(buildPlatform)
# When running inside Helix, the current directory is the "HelixTests" folder, so we look under: BuildOutput\$(buildConfiguration)\$(buildPlatform)
$certificates = Get-ChildItem -Recurse ".\BuildOutput\*\*\certificates\*.cer"
Write-Host "$($certificates.Length) found at .\BuildOutput\*\*\certificates\*.cer"
foreach ($cerFile in $certificates)
{
    Write-Host "Adding certificate '$cerFile'"
    certutil -addstore TrustedPeople "$cerFile"
}


if (Test-Path .\dotnet-windowsdesktop-runtime-installer.exe)
{
    Write-Host "Install dotnet runtime"
    .\dotnet-windowsdesktop-runtime-installer.exe /quiet /install /norestart /log dotnetinstalllog.txt |Out-Null
    Get-Content .\dotnetinstalllog.txt
    if ($env:HELIX_WORKITEM_UPLOAD_ROOT)
    {
        Copy-Item .\dotnetinstalllog.txt $env:HELIX_WORKITEM_UPLOAD_ROOT -Force
    }
}


# If we set the registry from a 32-bit process on a 64-bit machine, we will set the "virtualized" syswow registry.
# For crash dump collection we always want to set the "native" registry, so we make sure to invoke the native cmd.exe
$nativeCmdPath = "$env:SystemRoot\system32\cmd.exe"
if ([Environment]::Is64BitOperatingSystem -and ![Environment]::Is64BitProcess)
{
    # The "sysnative" path is a 'magic' path that allows a 32-bit process to invoke the native 64-bit cmd.exe.
    $nativeCmdPath = "$env:SystemRoot\sysnative\cmd.exe"
}

$dumpFolder = $env:HELIX_DUMP_FOLDER
if (!$dumpFolder)
{
    $dumpFolder = "C:\dumps"
}

function Enable-CrashDumpsForProcesses {
    Param([string[]]$namesOfProcessesForDumpCollection)

    foreach ($procName in $namesOfProcessesForDumpCollection )
    {
        Write-Host "Enabling local crash dumps for $procName"
        & $nativeCmdPath /c reg add "HKLM\Software\Microsoft\Windows\Windows Error Reporting\LocalDumps\$procName" /v DumpFolder /t REG_EXPAND_SZ /d $dumpFolder /f
        & $nativeCmdPath /c reg add "HKLM\Software\Microsoft\Windows\Windows Error Reporting\LocalDumps\$procName" /v DumpType /t REG_DWORD /d 2 /f
        & $nativeCmdPath /c reg add "HKLM\Software\Microsoft\Windows\Windows Error Reporting\LocalDumps\$procName" /v DumpCount /t REG_DWORD /d 3 /f
    }
}

# enable dump collection for our test apps:
$namesOfProcessesForDumpCollection = @(
    "te.exe",
    "te.processhost.exe"
)

Enable-CrashDumpsForProcesses $namesOfProcessesForDumpCollection

#Install VCRT
Get-ChildItem 'vc_redist.*.exe' | ForEach-Object {
  & $_.FullName /install /quiet /norestart
}

Get-ChildItem -Recurse "C:\Program Files\WindowsApps\*" > "$env:HELIX_WORKITEM_UPLOAD_ROOT\dump_dir_windowsapps.txt"
