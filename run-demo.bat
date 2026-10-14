@echo off
setlocal
cd /d "%~dp0"

echo ===========================================
echo Running FastDialog Windows 11 Native Demo
echo ===========================================
echo.

if not exist "target\FastDialog-0.1.0.jar" (
    echo [INFO] Building FastDialog first...
    call mvn clean package -DskipTests
)

cd examples\Demo
call mvn compile
cd ..\..

set "CP=target\classes;examples\Demo\target\classes;%USERPROFILE%\.m2\repository\com\github\andrestubbe\fastcore\0.1.0\fastcore-0.1.0.jar"

java -cp "%CP%" fastdialog.demo.Demo
pause
