:: //Script to copy main.exe into output folder and run it.

:: //If output folder does't exist script creates it.
:: //If main exec already exists in output then script doesn't copy and run only.
:: //If main exec does not exist then copy in output and run.
:: //If file already exists but -f(force) argument is passed then overrite and run.

@echo off 
set "sourceFileExecPath=src\main.exe" 
set "destFolder=output" 
set "fileName="
set "arg=%~1"
shift

if not exist "%destFolder%" (
    mkdir "%destFolder%" 
)

for %%i in ("%sourceFileExecPath%") do set fileName=%%~nxi

if not exist "%destFolder%\%fileName%" (
    copy "%sourceFileExecPath%" "%destFolder%"
) else if "%arg%" == "-f" (
    echo         Force Overwritting...
    copy "%sourceFileExecPath%" "%destFolder%"
) else (
    echo         File already Exists.
)

echo         Running...
"%destFolder%\%fileName%"