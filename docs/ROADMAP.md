# FastDialog Roadmap

## Milestone Status

### Windows 11 Shell COM & Native Dialog Engine (0.1.0)
**Status:** Released
- [x] Modern Windows 11 Open File Dialog via COM `IFileOpenDialog`.
- [x] Modern Windows 11 Save File Dialog via COM `IFileSaveDialog` with overwrite prompt.
- [x] Unlimited multi-file selection support via `IShellItemArray`.
- [x] Dedicated folder picker support (`FOS_PICKFOLDERS`).
- [x] Modern Windows 10/11 TaskDialog (`TaskDialogIndirect` with icons, instructions, and buttons).
- [x] Native Windows Color Chooser (`ChooseColorW`) returning 32-bit ARGB.
- [x] Windows Shell Reveal in Explorer (`SHOpenFolderAndSelectItems`).
- [x] Windows native Properties sheet invocation (`SHObjectProperties`).
- [x] Native Windows Font Chooser (`ChooseFontW`) returning family, pt size, bold/italic.
- [x] Modal anchoring to native parent windows (`HWND`).
- [x] Zero-bloat JNI bindings powered by `FastCore`.
- [x] Standardized individual demos & batch runners for every dialog flow.

---

## Upcoming Features

### Native Windows Print & Progress Dialogs (0.2.0)
**Status:** Planned
- [ ] Windows print dialog (`PrintDlgExW`) and page setup dialog (`PageSetupDlgW`).
- [ ] Native Windows Explorer Copy/Move/Delete Progress Dialog (`IFileOperation`).

### Cross-Platform Portal Abstraction (1.0.0)
**Status:** Backlog
- [ ] Linux XDG Desktop Portal file picker backend via D-Bus.
- [ ] macOS `NSOpenPanel` / `NSSavePanel` native bridge.
