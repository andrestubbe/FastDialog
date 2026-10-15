# Philosophy: FastDialog

The core design principles behind the zero-bloat, native-first architecture of **FastDialog**.

---

## 1. Native Shell vs. Swing Emulation

Java GUI toolkits traditionally try to emulate operating system file dialogues in pure Java (`JFileChooser`). This approach has major drawbacks on modern operating systems:

- **Missing Shell Namespaces**: Cloud storage providers like OneDrive, iCloud, and Dropbox are deeply integrated into the Windows Shell namespace. Emulated file trees cannot display selective sync status or pin state properly, often blocking the JVM event thread during file inspection.
- **Visual Disconnect**: Users expect modern Windows 11 Fluent chrome with search suggestions, Quick Access pins, and breadcrumb navigation bars.
- **Overhead**: Emulating a file browser requires instantiating large component hierarchies, file system monitors, and icon caches in Java heap space.

**FastDialog bypasses emulation completely** by delegating file interactions directly to the official Windows Shell COM interfaces (`IFileOpenDialog` and `IFileSaveDialog`).

---

## 2. Modern COM vs. Legacy Win32 APIs

Windows provides two native dialogue systems:
1. **Legacy Common Dialogs** (`GetOpenFileNameW` / `GetSaveFileNameW` via `commdlg32.dll`):
   - Dating back to Windows 95, these dialogs lack breadcrumb bars, Quick Access, and cloud drive awareness.
   - They rely on fixed-length caller-allocated string buffers. Selecting dozens of files in multi-selection mode frequently overflows the buffer or truncates paths.
2. **Modern Shell Item Dialogs** (`IFileOpenDialog` / `IFileSaveDialog`):
   - Introduced in Windows Vista and refined in Windows 11.
   - Handled asynchronously by the Windows Explorer shell.
   - Uses `IShellItem` and `IShellItemArray`, providing unlimited multi-selection capacity without buffer constraints.

FastDialog strictly leverages the modern COM interfaces for high reliability and modern aesthetics.

---

## 3. Zero-GC Latency Strategy

FastDialog is built around deterministic performance:
- COM lifecycle is managed via lightweight RAII wrappers (`ComScope`).
- Only the selected absolute file path strings are passed back across the JNI boundary.
- Zero auxiliary Java objects are created or retained during the dialog lifecycle.
