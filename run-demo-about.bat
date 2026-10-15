@echo off
setlocal
chcp 65001 > nul
cd /d "%~dp0"

echo ===========================================
echo FastDialog - Native About Box Demo
echo ===========================================
echo.

echo [+] Compiling FastDialog...
call mvn -q install -DskipTests > nul 2>&1

echo [+] Launching Demo...
cd examples\Demo
call mvn -q compile exec:java -Dexec.mainClass="fastdialog.demo.AboutDemo" -Djava.library.path="..\..\build"
if %ERRORLEVEL% NEQ 0 (
    echo.
    echo [ERROR] Demo failed to launch.
    pause
    exit /b %ERRORLEVEL%
)

cd ..\..
pause
