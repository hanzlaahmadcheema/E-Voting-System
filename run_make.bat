@echo off
echo [INFO] Running Makefile to build eVotingSystem...

where make >nul 2>nul
IF ERRORLEVEL 1 (
    echo [ERROR] 'make' is not found. Please install MSYS2 or MinGW and add it to PATH.
    pause
    exit /b
)

make

IF EXIST bin\eVotingSystem.exe (
    echo [INFO] Build successful. Launching eVotingSystem...
    bin\eVotingSystem.exe
) ELSE (
    echo [ERROR] Build failed. Executable not found.
)

pause
