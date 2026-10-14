# FastDialog 0.1.0 [ALPHA-2026-10-14] — Native Windows 11 Shell & COM Dialog Engine for Java

[![Status](https://img.shields.io/badge/status-0.1.0-brightgreen.svg)](https://github.com/andrestubbe/FastDialog/releases/tag/0.1.0)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Java](https://img.shields.io/badge/Java-17+-blue.svg)](https://www.java.com)
[![Platform](https://img.shields.io/badge/Platform-Windows%2010+-lightgrey.svg)]()
[![JitPack](https://img.shields.io/badge/JitPack-0.1.0-green.svg)](https://jitpack.io/#andrestubbe/FastDialog)

---

**⚡ High-performance native Windows 10/11 Shell & COM dialog engine for Java.**

**FastDialog** brings **authentic Windows 11 Explorer dialogs** directly to Java Swing, AWT, OpenGL, and CLI applications. It connects directly with the Windows Shell COM engine (`IFileOpenDialog` and `IFileSaveDialog`) to provide modern file pickers, multi-selection, folder pickers, and save dialogs with breadcrumb navigation, Quick Access / OneDrive pins, and native OS modality.

---

## Quick Start

### 1. Modern Windows 11 Open File Dialog
```java
import fastdialog.FastDialog;
import fastdialog.DialogFilter;

public class Demo {
    public static void main(String[] args) {
        // Open native Windows 11 file dialog with breadcrumbs and Quick Access
        String selectedFile = FastDialog.openFile(
            0, // Optional parent HWND
            "Select Source File",
            System.getProperty("user.home"),
            new DialogFilter("Java & Kotlin (*.java;*.kt)", "*.java;*.kt"),
            new DialogFilter("All Files (*.*)", "*.*")
        );

        if (selectedFile != null) {
            System.out.println("Selected: " + selectedFile);
        }
    }
}
```

### 2. Multi-File Selection
```java
String[] files = FastDialog.openFiles(
    0,
    "Select Multiple Files",
    "C:\\Projects",
    new DialogFilter("Text Files (*.txt;*.md)", "*.txt;*.md")
);
```

### 3. Folder Picker
```java
String folder = FastDialog.openFolder(0, "Choose Directory", "C:\\");
```

### 4. Save File Dialog with Extension Enforcement
```java
String savePath = FastDialog.saveFile(
    0,
    "Export Report",
    System.getProperty("user.home"),
    "report.pdf",
    "pdf",
    new DialogFilter("PDF Documents (*.pdf)", "*.pdf")
);
```

---

## Table of Contents

- [Why FastDialog?](#why-fastdialog)
- [Quick Start](#quick-start)
- [Key Features](#key-features)
- [API Quick Reference](#api-quick-reference)
- [Installation](#installation)
- [Documentation](#documentation)
- [Platform Support](#platform-support)
- [License](#license)
- [Related Projects](#related-projects)

---

## Why FastDialog?

Traditional Java Swing `JFileChooser` is infamous for looking outdated, rendering slowly, and missing core Windows Shell features:

| Feature | Java Swing `JFileChooser` | Legacy Win32 (`GetOpenFileNameW`) | FastDialog (`IFileOpenDialog`) |
|:---|:---|:---|:---|
| **Windows 11 UI** | ❌ Clunky Swing emulation | ❌ Windows 2000 style | **✅ Authentic Windows 11 Fluent Shell** |
| **Breadcrumbs & Quick Access**| ❌ Missing | ❌ Missing | **✅ Native Explorer Sidebar & Pins** |
| **OneDrive & Cloud Drives** | ⚠️ Unreliable / hangs | ⚠️ Partial | **✅ Full Shell Namespace Integration** |
| **Invocation Overhead** | Heavy AWT widget tree | Legacy dialog proc | **Direct sub-microsecond COM instantiation** |
| **Garbage Collection** | Heavy component allocation | C string conversions | **Zero GC beyond returned String paths** |
| **Dependencies** | Standard Java / Swing | Native bindings | **Pure Java 17+ backed by FastCore** |

---

## Key Features

- **🪟 Real Windows 11 Explorer Chrome** — Direct COM instantiation of `IFileOpenDialog` and `IFileSaveDialog`.
- **📂 Multi-Selection** — Efficient bulk file resolution via native `IShellItemArray`.
- **📁 Native Folder Picker** — Clean folder selection using `FOS_PICKFOLDERS`.
- **🛡️ Modal Window Anchoring** — Pass any native window handle (`HWND`) from `FastTheme`, `FastWindow`, or AWT to anchor the dialog modally.
- **⚡ Zero Bloat** — Lightweight package requiring only Java 17+ and `FastCore`.

---

## API Quick Reference

### FastDialog Facade (`fastdialog.FastDialog`)

| Method / Signature | Return Type | Description | Docs |
|:---|:---|:---|:---|
| `FastDialog.openFile(hwnd, title, folder, filters...)` | `String` | Opens modern Windows 11 file open dialog; returns absolute path or `null`. | [Reference](docs/REFERENCE.md) |
| `FastDialog.openFileObj(hwnd, title, folder, filters...)` | `File` | Same as `openFile` but returning a standard `java.io.File`. | [Reference](docs/REFERENCE.md) |
| `FastDialog.openFiles(hwnd, title, folder, filters...)` | `String[]` | Opens multi-select file dialog; returns array of selected paths. | [Reference](docs/REFERENCE.md) |
| `FastDialog.saveFile(hwnd, title, folder, name, ext, filters...)` | `String` | Opens modern Windows 11 save file dialog with overwrite prompt. | [Reference](docs/REFERENCE.md) |
| `FastDialog.openFolder(hwnd, title, folder)` | `String` | Opens native Windows folder picker dialog (`FOS_PICKFOLDERS`). | [Reference](docs/REFERENCE.md) |
| `FastDialog.isNativeAvailable()` | `boolean` | Checks if native `fastdialog.dll` JNI bridge is loaded and ready. | [Reference](docs/REFERENCE.md) |

---

## Installation

### Maven
```xml
<dependencies>
    <dependency>
        <groupId>com.github.andrestubbe</groupId>
        <artifactId>FastDialog</artifactId>
        <version>0.1.0</version>
    </dependency>
</dependencies>
```

---

## Platform Support

| Platform | Architecture | Status | Notes |
|:---|:---|:---|:---|
| Windows 10/11 | x64 | ✅ Fully Supported | Native COM `IFileOpenDialog` & `IFileSaveDialog` |
| Linux / macOS | Any | 🟡 Backlog | Planned native portal / file chooser integration |

---

## License

MIT License — See [LICENSE](LICENSE) for details.

---

## Related Projects

- [FastTheme](https://github.com/andrestubbe/FastTheme) — Native Windows 11 styling, Mica/Acrylic backdrops & theming
- [FastCore](https://github.com/andrestubbe/FastCore) — High-speed native JNI library loader
- [FastWindow](https://github.com/andrestubbe/FastWindow) — Lightweight Win32 native windowing engine
- [FastUI](https://github.com/andrestubbe/FastUI) — High-Performance GUI Framework

---

**Part of the FastJava Ecosystem** — *Making the JVM faster. Small package. Maximum speed. Zero bloat. 🚀📋*
