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

### TaskDialog
*   `public static int showTaskDialog(long parentHwnd, String title, String instruction, String content, int buttons, int iconId)`:
    *   Invokes the native Windows 10/11 `TaskDialogIndirect` with prominent header, body text, common buttons, and icons.
*   `public static void showInfo(String title, String instruction, String content)`: Shows information message.
*   `public static void showWarning(long parentHwnd, String title, String instruction, String content)`: Shows warning message.
*   `public static void showError(long parentHwnd, String title, String instruction, String content)`: Shows error message.
*   `public static boolean showConfirm(long parentHwnd, String title, String instruction, String content)`: Prompts user with YES/NO buttons, returns `true` if YES was pressed.

### Color Chooser
*   `public static int pickColor(long parentHwnd, int initialRgb)`:
    *   Opens the native Windows Color Chooser (`ChooseColorW`).
    *   Returns selected packed 32-bit ARGB integer (with alpha = 255), or `-1` if cancelled.

### Shell Integration
*   `public static boolean showInFolder(String filePath)`:
    *   Opens Windows Explorer and highlights the specified file (`SHOpenFolderAndSelectItems`).
*   `public static boolean showProperties(long parentHwnd, String filePath)`:
    *   Invokes the native Windows Properties sheet for a file or folder (`ShellExecuteExW` with verb `properties`).

### Font Chooser
*   `public static DialogFont pickFont(long parentHwnd, DialogFont initial)`:
    *   Opens the native Windows Font Chooser (`ChooseFontW`).
    *   Returns selected `DialogFont` object, or `null` if cancelled.

---

## 2. Class: `fastdialog.DialogFilter`

Defines file type filters displayed in the dialog's format drop-down.

### Constructor
*   `public DialogFilter(String description, String extensionPattern)`:
    *   `description`: Friendly name shown in drop-down (e.g. `"Image Files (*.png;*.jpg)"`).
    *   `extensionPattern`: Semicolon-delimited wildcards (e.g. `"*.png;*.jpg;*.jpeg"`).

---

## 3. Class: `fastdialog.DialogFont`

Encapsulates font typography attributes chosen from the native Windows Font Chooser.

### Properties & Methods
*   `public String getFamily()`: Font family name (e.g. `"Segoe UI"`, `"Cascadia Code"`).
*   `public int getSizePt()`: Font size in typographical points.
*   `public boolean isBold()`: Indicates bold font weight.
*   `public boolean isItalic()`: Indicates italic font posture.
