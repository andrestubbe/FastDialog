@echo off
setlocal
chcp 65001 > nul
cd /d "%~dp0"

echo ==================================================
echo   FastDialog Windows 11 Native Save File Demo
echo ==================================================
echo.

echo [+] Compiling FastDialog...
call mvn -q install -DskipTests > nul 2>&1

echo [+] Launching Save Demo...
cd examples\Demo
call mvn -q compile exec:java -Dexec.mainClass="fastdialog.demo.Save" -Djava.library.path="..\..\build"
if %ERRORLEVEL% NEQ 0 (
    echo.
    echo [ERROR] Demo failed to launch.
    pause
    exit /b %ERRORLEVEL%
)

cd ..\..
