# Changelog: FastDialog

All notable changes to this project will be documented in this file.

## [0.1.0] - 2026-10-14
### Added
- **Modern Windows 11 COM Dialog Engine**:
  - Direct integration with Windows Shell COM interfaces `IFileOpenDialog` and `IFileSaveDialog`.
  - True Windows 11 Explorer interface with breadcrumb navigation, Quick Access, OneDrive pins, and native OS modality.
- **Single File Open (`FastDialog.openFile`)**:
  - Modal parent HWND anchoring.
  - Custom title and initial folder setup.
  - Flexible `DialogFilter` format specifications.
- **Multi-File Open (`FastDialog.openFiles`)**:
  - `FOS_ALLOWMULTISELECT` support returning arrays of selected paths via `IShellItemArray`.
- **Folder Picker (`FastDialog.openFolder`)**:
  - Directory-only selection via `FOS_PICKFOLDERS`.
- **Save File Dialog (`FastDialog.saveFile`)**:
  - Overwrite prompt (`FOS_OVERWRITEPROMPT`), default file names, and default extension enforcement.
- **Zero-Bloat JNI Architecture**:
  - Direct C++ Win32/COM bridge with unified `FastCore` native loader.
  - Interactive test suite and demo showcase (`examples/Demo`).
