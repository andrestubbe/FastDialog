# FastDialog Roadmap

## Milestone Status

### Windows 11 Shell COM Dialog Engine (0.1.0)
**Status:** Released
- [x] Modern Windows 11 Open File Dialog via COM `IFileOpenDialog`.
- [x] Modern Windows 11 Save File Dialog via COM `IFileSaveDialog` with overwrite prompt.
- [x] Unlimited multi-file selection support via `IShellItemArray`.
- [x] Dedicated folder picker support (`FOS_PICKFOLDERS`).
- [x] Flexible file filter definitions via `DialogFilter`.
- [x] Modal anchoring to native parent windows (`HWND`).
- [x] Zero-bloat JNI bindings powered by `FastCore`.
- [x] Interactive showcase demo (`Demo.java`).

---

## Upcoming Features

### Native Color & Font Dialogs (0.2.0)
**Status:** Planned
- [ ] Windows native color chooser dialog (`ChooseColorW`).
- [ ] Windows native font chooser dialog (`ChooseFontW`) returning family, point size, weight, and style.

### Native Windows Print & Property Dialogs (0.3.0)
**Status:** Planned
- [ ] Windows print dialog (`PrintDlgExW`) and page setup dialog.
- [ ] Windows Explorer properties sheet invocation (`SHObjectProperties`).
- [ ] Native shell context menu delegation.

### Cross-Platform Portal Abstraction (1.0.0)
**Status:** Backlog
- [ ] Linux XDG Desktop Portal file picker backend via D-Bus.
- [ ] macOS `NSOpenPanel` / `NSSavePanel` native bridge.
