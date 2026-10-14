# FastDialog API Reference

This document outlines the API contracts, data structures, and methods of the **FastDialog** engine (version 0.1.0).

---

## 1. Class: `fastdialog.FastDialog`

Primary static facade for invoking modern Windows Shell COM dialogs (`IFileOpenDialog` and `IFileSaveDialog`).

### Native Availability
*   `public static boolean isNativeAvailable()`: Returns `true` if `fastdialog.dll` was successfully loaded by `FastCore`.
*   `public static Throwable getLoadError()`: Returns the exception encountered if native loading failed.

### Open File Dialogs
*   `public static String openFile(long parentHwnd, String title, String defaultFolder, DialogFilter... filters)`:
    *   Displays a modern Windows 11 `IFileOpenDialog`.
    *   Anchored to `parentHwnd` as a modal child (or floating if `parentHwnd == 0`).
    *   Returns the absolute filesystem path of the selected file, or `null` if cancelled.
*   `public static String openFile(long parentHwnd)`: Simplified overload with default title and initial folder.
*   `public static String openFile()`: Convenience overload without parent window.
*   `public static File openFileObj(long parentHwnd, String title, String defaultFolder, DialogFilter... filters)`: Same as `openFile` returning a `java.io.File`.

### Multiple File Open Dialogs
*   `public static String[] openFiles(long parentHwnd, String title, String defaultFolder, DialogFilter... filters)`:
    *   Opens an `IFileOpenDialog` configured with `FOS_ALLOWMULTISELECT`.
    *   Iterates the resulting `IShellItemArray` to extract all selected paths.
    *   Returns an array of absolute paths, or `null` if cancelled.
*   `public static String[] openFiles(long parentHwnd)`: Simplified multi-select overload.
*   `public static String[] openFiles()`: Simplified multi-select overload without parent window.

### Save File Dialogs
*   `public static String saveFile(long parentHwnd, String title, String defaultFolder, String defaultName, String defaultExt, DialogFilter... filters)`:
    *   Displays a modern Windows 11 `IFileSaveDialog` with `FOS_OVERWRITEPROMPT`.
    *   Pre-fills `defaultName` and enforces `defaultExt`.
    *   Returns the chosen target path, or `null` if cancelled.
*   `public static String saveFile(long parentHwnd, String defaultName, DialogFilter... filters)`: Simplified save overload.
*   `public static String saveFile()`: Simplified save overload without parent window.

### Folder Picker Dialogs
*   `public static String openFolder(long parentHwnd, String title, String defaultFolder)`:
    *   Displays an `IFileOpenDialog` configured with `FOS_PICKFOLDERS`.
    *   Allows folder selection only.
    *   Returns the absolute folder path, or `null` if cancelled.
*   `public static String openFolder(long parentHwnd)`: Simplified folder picker overload.
*   `public static String openFolder()`: Simplified folder picker overload without parent window.

---

## 2. Class: `fastdialog.DialogFilter`

Defines file type filters displayed in the dialog's format drop-down.

### Constructor
*   `public DialogFilter(String description, String extensionPattern)`:
    *   `description`: Friendly name shown in drop-down (e.g. `"Image Files (*.png;*.jpg)"`).
    *   `extensionPattern`: Semicolon-delimited wildcards (e.g. `"*.png;*.jpg;*.jpeg"`).
