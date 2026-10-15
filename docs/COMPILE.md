# Compilation Guide: FastDialog

This guide provides instructions for compiling the native C++ Win32/COM bridge and packaging the FastDialog JAR.

---

## Prerequisites

1. **Java Development Kit (JDK)**: JDK 17 or higher (tested with JDK 21).
2. **Visual Studio**: Visual Studio 2022 (Community or higher) with the **Desktop development with C++** workload installed.
3. **Apache Maven**: Version 3.8+ for Java building and packaging.
4. **FastCore**: Installed in your local Maven repository (`mvn install` on FastCore).

---

## 1. Quick Build (Batch Script)

The easiest way to compile both the native DLL and the Java artifact is using the bundled build script:

```cmd
.\compile.bat
```

This script:
1. Locates Visual Studio automatically using `vswhere.exe`.
2. Initializes the x64 native MSVC tools environment (`vcvars64.bat`).
3. Compiles `native\FastDialog.cpp` into `build\fastdialog.dll` linking against `user32.lib`, `ole32.lib`, `shell32.lib`, and `shlwapi.lib`.
4. Deploys `fastdialog.dll` to:
   - `release/fastdialog.dll`
   - `src/main/resources/fastdialog.dll`
   - `src/main/resources/native/fastdialog.dll`
   - `%USERPROFILE%/.fastcore/native/fastdialog/fastdialog.dll`
5. Invokes `mvn install -DskipTests` to install the final artifact to your local Maven repository (`~/.m2`).

---

## 2. Manual C++ Compilation (MSVC)

If compiling from a Visual Studio Developer Command Prompt:

```cmd
cl /LD /Fe:build\fastdialog.dll /Fo:build\ ^
    native\FastDialog.cpp ^
    user32.lib ole32.lib shell32.lib shlwapi.lib oleaut32.lib ^
    /I"%JAVA_HOME%\include" ^
    /I"%JAVA_HOME%\include\win32" ^
    /EHsc /std:c++17 /O2 /W3 ^
    /link /LIBPATH:"%JAVA_HOME%\lib" /DEF:native\FastDialog.def
```

---

## 3. Java Packaging via Maven

```cmd
mvn clean package -DskipTests
```
