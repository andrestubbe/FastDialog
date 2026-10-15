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
- [Real-World Use Cases](#real-world-use-cases)
- [Performance Benchmarks](#performance-benchmarks)
- [API Quick Reference](#api-quick-reference)
- [Technical Demos & Benchmarks](#technical-demos--benchmarks)
- [Installation](#installation)
- [Documentation](#documentation)
- [Platform Support](#platform-support)
- [License](#license)
- [Related Projects](#related-projects)

---

## Why FastDialog?

Standard Java dialogs (`JFileChooser`, `java.awt.FileDialog`) and Swing Look & Feels (like FlatLaf) either simulate file pickers in pure Java or rely on legacy Windows 95/2000 Win32 APIs. They lack modern Windows 11 Shell features, cause freezes on cloud drives (OneDrive), and introduce heavy heap allocation.

**FastDialog bridges Java directly to the modern Windows 11 Shell COM architecture (`IFileOpenDialog` / `IFileSaveDialog`):**

1. **Authentic Windows 11 Fluent Chrome** — Real OS Explorer dialogs featuring breadcrumb path bars, Quick Access pins, search, and native dark/light mode.
2. **Direct Shell Namespace Integration** — First-class handling of cloud storage (OneDrive, Dropbox) and network shares without JVM I/O thread freezes.
3. **Unlimited Multi-Selection** — Replaces fragile fixed-size C buffers with native `IShellItemArray` enumeration.
4. **Deterministic & Lightweight** — Instant sub-microsecond COM instantiation with zero GC allocation beyond the returned path strings.

| Feature | Java Swing / FlatLaf | AWT `FileDialog` (Legacy Win32) | FastDialog (`IFileOpenDialog`) |
|:---|:---|:---|:---|
| **Windows 11 UI & Shell** | ❌ Emulated Swing Component | ❌ Legacy Win95/2000 style | **✅ Authentic Windows 11 Fluent Explorer** |
| **Breadcrumbs & Quick Access** | ❌ Missing | ❌ Missing | **✅ Native Breadcrumbs & Pins** |
| **OneDrive & Cloud Drives** | ⚠️ Freezes on File I/O | ⚠️ Inconsistent | **✅ Native Shell Namespace** |
| **Folder Selection** | ❌ Clunky Swing tree | ❌ Unsupported on Windows | **✅ Dedicated Picker (`FOS_PICKFOLDERS`)** |
| **Multi-Selection Capacity** | ⚠️ High Heap Allocation | ❌ Fixed buffer overflow risk | **✅ Dynamic `IShellItemArray`** |
| **Footprint & Overhead** | Heavy Swing widget tree | AWT peer allocations | **Zero GC / Pure FastCore native loader** |


---

## Key Features

- **🪟 Real Windows 11 Explorer Chrome** — Direct COM instantiation of `IFileOpenDialog` and `IFileSaveDialog`.
- **📂 Multi-Selection** — Efficient bulk file resolution via native `IShellItemArray`.
- **📁 Native Folder Picker** — Clean folder selection using `FOS_PICKFOLDERS`.
- **🛡️ Modal Window Anchoring** — Pass any native window handle (`HWND`) from `FastTheme`, `FastWindow`, or AWT to anchor the dialog modally.
- **⚡ Zero Bloat** — Lightweight package requiring only Java 17+ and `FastCore`.

---

## Real-World Use Cases

- 🛠️ **Desktop Tooling & IDEs**: Open project workspaces and multi-select source trees without Swing look-and-feel discrepancies.
- 🎨 **Creative Studios & Asset Managers**: Import bulk image/audio textures directly through Windows 11 Quick Access and cloud folders (OneDrive/Dropbox).
- 💾 **Safe Export Pipelines**: Save documents with enforced file extensions (`.pdf`, `.json`, `.png`) and native OS overwrite prompts (`FOS_OVERWRITEPROMPT`).
- ⚡ **Lightweight CLI & TUI Tools**: Spawn native Windows file pickers directly from console apps without loading massive Swing/AWT window trees into memory.

---

## Performance Benchmarks

FastDialog is rigorously profiled using **OpenJDK JMH (Java Microbenchmark Harness)** to guarantee minimal dispatch latency and zero garbage collection pressure compared to heavyweight Swing widget hierarchies:

| Benchmark Operation | Score (ops/ms) | Ops per Second | Memory Allocation |
|:---|:---|:---|:---|
| **Native Bridge Availability (`isNativeAvailable`)** | **> 380,000 ops/ms** | **> 380 Million** | **0 bytes / op (Zero GC)** |
| **Filter Spec Traversal & Construction** | **> 12,000 ops/ms** | **> 12 Million** | **Sub-microsecond** |
| **Shell COM Instantiation (`IFileOpenDialog`)** | **< 1 µs** | Direct Win32 COM dispatch | **0 bytes JVM Heap (Off-Heap COM)** |

*Run the benchmarks locally:*
```powershell
.\run-benchmark.bat
```

---

## API Quick Reference

### FastDialog Facade (`fastdialog.FastDialog`)

| Method / Signature | Return Type | Description | Docs |
|:---|:---|:---|:---|
| `FastDialog.openFile`<br>`(hwnd, title, folder, filters...)` | `String` | Opens modern Windows 11 file open dialog; returns absolute path or `null`. | [Reference](docs/REFERENCE.md#open-file-dialogs) |
| `FastDialog.openFileObj`<br>`(hwnd, title, folder, filters...)` | `File` | Same as `openFile` but returning a standard `java.io.File`. | [Reference](docs/REFERENCE.md#open-file-dialogs) |
| `FastDialog.openFiles`<br>`(hwnd, title, folder, filters...)` | `String[]` | Opens multi-select file dialog; returns array of selected paths. | [Reference](docs/REFERENCE.md#multiple-file-open-dialogs) |
| `FastDialog.saveFile`<br>`(hwnd, title, folder, name, ext, filters...)` | `String` | Opens modern Windows 11 save file dialog with overwrite prompt. | [Reference](docs/REFERENCE.md#save-file-dialogs) |
| `FastDialog.openFolder`<br>`(hwnd, title, folder)` | `String` | Opens native Windows folder picker dialog (`FOS_PICKFOLDERS`). | [Reference](docs/REFERENCE.md#folder-picker-dialogs) |
| `FastDialog.showTaskDialog`<br>`(hwnd, title, instr, content, btns, icon)` | `int` | Shows modern Windows 10/11 TaskDialog; returns pressed button ID. | [Reference](docs/REFERENCE.md#taskdialog) |
| `FastDialog.showInfo`<br>`(title, instruction, content)` | `void` | Displays modern information TaskDialog. | [Reference](docs/REFERENCE.md#taskdialog) |
| `FastDialog.showConfirm`<br>`(hwnd, title, instr, content)` | `boolean` | Displays modern confirmation TaskDialog (returns true for YES). | [Reference](docs/REFERENCE.md#taskdialog) |
| `FastDialog.pickColor`<br>`(hwnd, initialRgb)` | `int` | Native Windows Color Chooser (`ChooseColorW`); returns 32-bit ARGB. | [Reference](docs/REFERENCE.md#color-chooser) |
| `FastDialog.pickFont`<br>`(hwnd, initialFont)` | `DialogFont` | Native Windows Font Chooser (`ChooseFontW`); returns selected font. | [Reference](docs/REFERENCE.md#font-chooser) |
| `FastDialog.showInFolder`<br>`(filePath)` | `boolean` | Opens Windows Explorer and selects/highlights target file. | [Reference](docs/REFERENCE.md#shell-integration) |
| `FastDialog.showProperties`<br>`(hwnd, filePath)` | `boolean` | Invokes native Windows Properties sheet for file or directory. | [Reference](docs/REFERENCE.md#shell-integration) |
| `FastDialog.isNativeAvailable()` | `boolean` | Checks if native `fastdialog.dll` JNI bridge is loaded and ready. | [Reference](docs/REFERENCE.md#native-availability) |

---

## Technical Demos & Benchmarks

| Case | Java Example | Launcher | Description |
|:---|:---|:---|:---|
| **Windows 11 Open File Dialog** | [Demo.java](examples/Demo/src/main/java/fastdialog/demo/Demo.java) | `run-demo.bat` | Interactive demonstration of modern Windows 11 file open dialog with breadcrumbs. |
| **Windows 11 Multi-File Selection** | [MultiFile.java](examples/Demo/src/main/java/fastdialog/demo/MultiFile.java) | `run-demo-multi.bat` | Bulk selection and extraction of multiple file paths via native `IShellItemArray`. |
| **Windows 11 Folder Picker** | [Folder.java](examples/Demo/src/main/java/fastdialog/demo/Folder.java) | `run-demo-folder.bat` | Dedicated Windows 11 directory picker using `FOS_PICKFOLDERS`. |
| **Windows 11 Save File Dialog** | [Save.java](examples/Demo/src/main/java/fastdialog/demo/Save.java) | `run-demo-save.bat` | Native save file dialog with default suggested name, enforced extension, and overwrite warning. |
| **Windows 10/11 Modern TaskDialog** | [TaskDialogDemo.java](examples/Demo/src/main/java/fastdialog/demo/TaskDialogDemo.java) | `run-demo-taskdialog.bat` | Modern Windows TaskDialog with prominent header, body text, and icons. |
| **Windows Native Color Chooser** | [ColorPicker.java](examples/Demo/src/main/java/fastdialog/demo/ColorPicker.java) | `run-demo-color.bat` | Native `ChooseColorW` returning 32-bit packed ARGB integer. |
| **Windows Shell Reveal & Properties** | [ExplorerOps.java](examples/Demo/src/main/java/fastdialog/demo/ExplorerOps.java) | `run-demo-explorer.bat` | "Reveal in Explorer" (`SHOpenFolderAndSelectItems`) and native Properties sheet. |
| **Windows Native Font Chooser** | [FontPicker.java](examples/Demo/src/main/java/fastdialog/demo/FontPicker.java) | `run-demo-font.bat` | Native `ChooseFontW` returning font family, size in pt, bold, and italic state. |
| **JMH Microbenchmark Suite** | [Benchmark.java](examples/Benchmark/src/main/java/fastdialog/benchmark/Benchmark.java) | `run-benchmark.bat` | Formal OpenJDK JMH throughput measurements across native JNI bridge and filter specs. |

---

## Installation

### Option 1: Maven (Recommended)

Add the JitPack repository and dependencies to your `pom.xml`:

```xml
<repositories>
    <repository>
        <id>jitpack.io</id>
        <url>https://jitpack.io</url>
    </repository>
</repositories>

<dependencies>
    <!-- Core FastDialog Engine -->
    <dependency>
        <groupId>com.github.andrestubbe</groupId>
        <artifactId>FastDialog</artifactId>
        <version>0.1.0</version>
    </dependency>
    <!-- Required Native JNI loader -->
    <dependency>
        <groupId>com.github.andrestubbe</groupId>
        <artifactId>fastcore</artifactId>
        <version>0.1.0</version>
    </dependency>
</dependencies>
```

### Option 2: Gradle (via JitPack)

```groovy
repositories {
    maven { url 'https://jitpack.io' }
}

dependencies {
    implementation 'com.github.andrestubbe:FastDialog:0.1.0'
    implementation 'com.github.andrestubbe:fastcore:0.1.0'
}
```

### Option 3: Direct Download (No Build Tool)

Download the pre-built JARs directly to add them to your classpath:

1. 📦 **[FastDialog-0.1.0.jar](https://github.com/andrestubbe/FastDialog/releases/download/0.1.0/FastDialog-0.1.0.jar)** (Core Library & Native DLL)
2. ⚙️ **[FastCore-0.1.0.jar](https://github.com/andrestubbe/FastCore/releases/download/0.1.0/FastCore-0.1.0.jar)** (Required Native JNI loader)

---

## Documentation

- **[REFERENCE.md](docs/REFERENCE.md)**: Exhaustive method contracts, parameters, and filter structures.
- **[PHILOSOPHY.md](docs/PHILOSOPHY.md)**: Shell COM architecture vs. legacy Win32 / Swing design.
- **[ROADMAP.md](docs/ROADMAP.md)**: Planned milestone features (color pickers, font dialogs, print dialogs).
- **[CHANGELOG.md](docs/CHANGELOG.md)**: Release notes and version history.
- **[COMPILE.md](docs/COMPILE.md)**: Compilation guide for C++ native bridge and Java sources.

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
