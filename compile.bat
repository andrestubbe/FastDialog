@echo off
setlocal EnableDelayedExpansion

:: Change to script directory
cd /d "%~dp0"

echo ===========================================
echo FastDialog JNI Bridge Build Script
echo ===========================================
echo.
echo Running in: %CD%
echo.

:: Check for Java
if not defined JAVA_HOME (
    if exist "C:\Program Files\Java\jdk-21.0.12.1\include\jni.h" (
        set "JAVA_HOME=C:\Program Files\Java\jdk-21.0.12.1"
    ) else if exist "C:\Program Files\Java\latest\include\jni.h" (
        set "JAVA_HOME=C:\Program Files\Java\latest"
    ) else if exist "C:\Program Files\Java\jdk-25.0.3\include\jni.h" (
        set "JAVA_HOME=C:\Program Files\Java\jdk-25.0.3"
    )
)

if not exist "%JAVA_HOME%\include\jni.h" (
    echo ERROR: Cannot find jni.h in %JAVA_HOME%\include
    echo Please check your Java installation
    pause
    exit /b 1
)

:: Use vswhere to find Visual Studio
set "VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"

if not exist "%VSWHERE%" (
    echo ERROR: vswhere.exe not found!
    echo Visual Studio Installer might be missing.
    echo.
    pause
    exit /b 1
)

:: Find VS installation path
for /f "usebackq tokens=*" %%i in (`"%VSWHERE%" -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath`) do (
    set "VS_INSTALL=%%i"
)

if not defined VS_INSTALL (
    echo ERROR: Visual Studio with C++ tools not found!
    echo.
    pause
    exit /b 1
)

echo Found Visual Studio at: %VS_INSTALL%

:: Setup VS environment
set "VCVARS=%VS_INSTALL%\VC\Auxiliary\Build\vcvars64.bat"

echo Setting up Visual Studio environment...
call "%VCVARS%"
if errorlevel 1 (
    echo ERROR: Failed to setup VS environment
    pause
    exit /b 1
)

:: Create build directories
if not exist build mkdir build
if not exist build\classes mkdir build\classes

:: Compile JNI DLL
echo.
echo Compiling FastDialog JNI Bridge (C++)...
echo =====================================================
cl /LD /Fe:build\fastdialog.dll /Fo:build\ ^
    native\FastDialog.cpp ^
    user32.lib gdi32.lib ole32.lib shell32.lib shlwapi.lib oleaut32.lib comctl32.lib comdlg32.lib credui.lib ^
    /I"%JAVA_HOME%\include" ^
    /I"%JAVA_HOME%\include\win32" ^
    /EHsc /std:c++17 /O2 /W3 ^
    /link /LIBPATH:"%JAVA_HOME%\lib" /DEF:native\FastDialog.def

if %errorlevel% neq 0 (
    echo.
    echo =====================================================
    echo C++ COMPILATION FAILED
    echo =====================================================
    pause
    exit /b 1
)

:: Copy DLL to all required target locations
echo.
echo Copying DLL to release, resources, and .fastcore cache...
if not exist "release" mkdir release
if not exist "src\main\resources\native" mkdir "src\main\resources\native"
if not exist "src\main\resources\win32-x86-64" mkdir "src\main\resources\win32-x86-64"
if not exist "target\classes\native" mkdir "target\classes\native"
set "FASTCORE_DIR=%USERPROFILE%\.fastcore\native\fastdialog"
if not exist "!FASTCORE_DIR!" mkdir "!FASTCORE_DIR!"

copy /Y build\fastdialog.dll release\fastdialog.dll >nul
copy /Y build\fastdialog.dll src\main\resources\fastdialog.dll >nul
copy /Y build\fastdialog.dll src\main\resources\native\fastdialog.dll >nul
copy /Y build\fastdialog.dll src\main\resources\win32-x86-64\fastdialog.dll >nul
copy /Y build\fastdialog.dll target\classes\native\fastdialog.dll >nul 2>&1
copy /Y build\fastdialog.dll target\classes\fastdialog.dll >nul 2>&1
copy /Y build\fastdialog.dll "!FASTCORE_DIR!\fastdialog.dll" >nul
powershell -NoProfile -Command "Unblock-File -Path '!FASTCORE_DIR!\fastdialog.dll', 'release\fastdialog.dll', 'src\main\resources\native\fastdialog.dll' -ErrorAction SilentlyContinue" >nul 2>&1

:: Cleanup temporary native artifacts
del /Q build\*.obj
del /Q build\*.exp
del /Q build\*.lib

:: Compile Java and Install to Local Maven Repo
echo.
echo Compiling Java and Installing to Local Maven Repo...
call mvn install -DskipTests
if %errorlevel% neq 0 (
    echo.
    echo =====================================================
    echo MAVEN BUILD FAILED
    echo =====================================================
    echo Ensure Maven is installed and FastCore is accessible.
    pause
    exit /b 1
)

:: Success
echo.
echo =====================================================
echo BUILD SUCCESSFUL! (v0.1.0)
echo =====================================================
echo.
echo FastDialog JNI Bridge created with:
echo - Windows 11 Modern COM IFileOpenDialog (Breadcrumb, QuickAccess)
echo - Multi-Select file opening (IShellItemArray)
echo - Windows 11 Modern IFileSaveDialog
echo - Native Folder Picker (FOS_PICKFOLDERS)
echo.
echo Standard JAR: target/FastDialog-0.1.0.jar
echo Native DLL  : release/fastdialog.dll
echo.
pause
