package fastdialog;

import fastcore.FastCore;
import java.io.File;

/**
 * High-performance native Windows 10/11 Shell and COM dialog engine for Java.
 * <p>
 * Provides direct access to modern Windows 11 Explorer file open, file save, and folder picker
 * dialogs ({@code IFileOpenDialog} and {@code IFileSaveDialog}) with zero external runtime bloat.
 */
public final class FastDialog {

    private static volatile boolean nativeLoaded = false;
    private static volatile Throwable loadError = null;

    static {
        try {
            // Load native library via FastCore unified loader
            FastCore.loadLibrary("fastdialog");
            nativeLoaded = true;
        } catch (Throwable t) {
            loadError = t;
            // Fallback attempt: standard System.loadLibrary
            try {
                System.loadLibrary("fastdialog");
                nativeLoaded = true;
                loadError = null;
            } catch (Throwable t2) {
                // Keep loadError
            }
        }
    }

    private FastDialog() {
        // Pure static utility facade
    }

    /**
     * Checks if the native Windows dialog DLL was successfully loaded.
     */
    public static boolean isNativeAvailable() {
        return nativeLoaded;
    }

    /**
     * Returns the initialization error if loading failed, or null if successful.
     */
    public static Throwable getLoadError() {
        return loadError;
    }

    // =========================================================================
    // High-Level Open File Dialogs
    // =========================================================================

    /**
     * Opens a modern Windows 11 Open File Dialog.
     *
     * @param parentHwnd    Native window handle (HWND) of the parent window (0 for independent).
     * @param title         Dialog title window text (or null for default).
     * @param defaultFolder Initial directory path (or null for default/recent).
     * @param filters       Array of {@link DialogFilter} specifications.
     * @return Selected absolute file path, or {@code null} if cancelled by the user.
     */
    public static String openFile(long parentHwnd, String title, String defaultFolder, DialogFilter... filters) {
        String[] descs = null;
        String[] exts = null;
        if (filters != null && filters.length > 0) {
            descs = new String[filters.length];
            exts = new String[filters.length];
            for (int i = 0; i < filters.length; i++) {
                descs[i] = filters[i].getDescription();
                exts[i] = filters[i].getExtensionPattern();
            }
        }
        return nativeOpenFile(parentHwnd, title, defaultFolder, descs, exts);
    }

    /**
     * Convenience method: Opens an Open File Dialog with default settings.
     */
    public static String openFile(long parentHwnd) {
        return openFile(parentHwnd, null, null);
    }

    /**
     * Convenience method: Opens an Open File Dialog without parent window.
     */
    public static String openFile() {
        return openFile(0, null, null);
    }

    /**
     * Convenience method: Opens an Open File Dialog and returns a {@link File} object.
     */
    public static File openFileObj(long parentHwnd, String title, String defaultFolder, DialogFilter... filters) {
        String path = openFile(parentHwnd, title, defaultFolder, filters);
        return path != null ? new File(path) : null;
    }

    // =========================================================================
    // High-Level Multiple File Open Dialogs
    // =========================================================================

    /**
     * Opens a modern Windows 11 Multiple File Selection Dialog.
     *
     * @param parentHwnd    Native window handle (HWND) of the parent window (0 for independent).
     * @param title         Dialog title window text (or null for default).
     * @param defaultFolder Initial directory path (or null for default/recent).
     * @param filters       Array of {@link DialogFilter} specifications.
     * @return Array of selected absolute file paths, or {@code null} if cancelled.
     */
    public static String[] openFiles(long parentHwnd, String title, String defaultFolder, DialogFilter... filters) {
        String[] descs = null;
        String[] exts = null;
        if (filters != null && filters.length > 0) {
            descs = new String[filters.length];
            exts = new String[filters.length];
            for (int i = 0; i < filters.length; i++) {
                descs[i] = filters[i].getDescription();
                exts[i] = filters[i].getExtensionPattern();
            }
        }
        return nativeOpenFiles(parentHwnd, title, defaultFolder, descs, exts);
    }

    public static String[] openFiles(long parentHwnd) {
        return openFiles(parentHwnd, null, null);
    }

    public static String[] openFiles() {
        return openFiles(0, null, null);
    }

    // =========================================================================
    // High-Level Save File Dialogs
    // =========================================================================

    /**
     * Opens a modern Windows 11 Save File Dialog.
     *
     * @param parentHwnd    Native window handle (HWND) of the parent window (0 for independent).
     * @param title         Dialog title window text (or null for default).
     * @param defaultFolder Initial directory path (or null for default/recent).
     * @param defaultName   Default suggested file name (or null).
     * @param defaultExt    Default file extension appended if omitted (e.g. "png" or "txt").
     * @param filters       Array of {@link DialogFilter} specifications.
     * @return Selected absolute save destination path, or {@code null} if cancelled.
     */
    public static String saveFile(long parentHwnd, String title, String defaultFolder, String defaultName, String defaultExt, DialogFilter... filters) {
        String[] descs = null;
        String[] exts = null;
        if (filters != null && filters.length > 0) {
            descs = new String[filters.length];
            exts = new String[filters.length];
            for (int i = 0; i < filters.length; i++) {
                descs[i] = filters[i].getDescription();
                exts[i] = filters[i].getExtensionPattern();
            }
        }
        return nativeSaveFile(parentHwnd, title, defaultFolder, defaultName, defaultExt, descs, exts);
    }

    public static String saveFile(long parentHwnd, String defaultName, DialogFilter... filters) {
        return saveFile(parentHwnd, null, null, defaultName, null, filters);
    }

    public static String saveFile() {
        return saveFile(0, null, null, null, null, (DialogFilter[]) null);
    }

    // =========================================================================
    // High-Level Folder Picker Dialogs
    // =========================================================================

    /**
     * Opens a modern Windows 11 Folder Picker Dialog (using {@code IFileOpenDialog} with {@code FOS_PICKFOLDERS}).
     *
     * @param parentHwnd    Native window handle (HWND) of the parent window (0 for independent).
     * @param title         Dialog title window text (or null for default).
     * @param defaultFolder Initial directory path (or null for default/recent).
     * @return Selected directory path, or {@code null} if cancelled.
     */
    public static String openFolder(long parentHwnd, String title, String defaultFolder) {
        return nativeOpenFolder(parentHwnd, title, defaultFolder);
    }

    public static String openFolder(long parentHwnd) {
        return openFolder(parentHwnd, null, null);
    }

    public static String openFolder() {
        return openFolder(0, null, null);
    }

    // =========================================================================
    // High-Level TaskDialog (Modern Windows 10/11 MessageBox)
    // =========================================================================

    public static final int BUTTON_OK = 0x0001;
    public static final int BUTTON_YES = 0x0002;
    public static final int BUTTON_NO = 0x0004;
    public static final int BUTTON_CANCEL = 0x0008;
    public static final int BUTTON_RETRY = 0x0010;
    public static final int BUTTON_CLOSE = 0x0020;

    public static final int ICON_NONE = 0;
    public static final int ICON_INFO = 1;
    public static final int ICON_WARNING = 2;
    public static final int ICON_ERROR = 3;
    public static final int ICON_SHIELD = 4;

    public static final int RESULT_OK = 1;
    public static final int RESULT_CANCEL = 2;
    public static final int RESULT_RETRY = 4;
    public static final int RESULT_YES = 6;
    public static final int RESULT_NO = 7;
    public static final int RESULT_CLOSE = 8;

    /**
     * Shows a modern Windows 10/11 TaskDialog.
     *
     * @param parentHwnd    Parent window handle (or 0).
     * @param title         Window title text.
     * @param instruction   Prominent main instruction header (e.g. "Operation Completed").
     * @param content       Detailed explanatory body text.
     * @param buttons       Bitwise OR of BUTTON_* constants.
     * @param iconId        ICON_* constant.
     * @return Result button ID (RESULT_*).
     */
    public static int showTaskDialog(long parentHwnd, String title, String instruction, String content, int buttons, int iconId) {
        return nativeShowTaskDialog(parentHwnd, title, instruction, content, buttons, iconId);
    }

    public static void showInfo(long parentHwnd, String title, String instruction, String content) {
        showTaskDialog(parentHwnd, title, instruction, content, BUTTON_OK, ICON_INFO);
    }

    public static void showInfo(String title, String instruction, String content) {
        showInfo(0, title, instruction, content);
    }

    public static void showWarning(long parentHwnd, String title, String instruction, String content) {
        showTaskDialog(parentHwnd, title, instruction, content, BUTTON_OK, ICON_WARNING);
    }

    public static void showError(long parentHwnd, String title, String instruction, String content) {
        showTaskDialog(parentHwnd, title, instruction, content, BUTTON_OK, ICON_ERROR);
    }

    public static boolean showConfirm(long parentHwnd, String title, String instruction, String content) {
        int res = showTaskDialog(parentHwnd, title, instruction, content, BUTTON_YES | BUTTON_NO, ICON_INFO);
        return res == RESULT_YES;
    }

    // =========================================================================
    // High-Level Native Color Chooser
    // =========================================================================

    /**
     * Opens the native Windows Color Chooser (ChooseColorW).
     *
     * @param parentHwnd Parent window handle (or 0).
     * @param initialRgb Initial packed RGB/ARGB color (e.g. 0x00E0FF).
     * @return Selected 32-bit packed ARGB color (with alpha = 255), or -1 if cancelled.
     */
    public static int pickColor(long parentHwnd, int initialRgb) {
        return nativePickColor(parentHwnd, initialRgb);
    }

    public static int pickColor(int initialRgb) {
        return pickColor(0, initialRgb);
    }

    public static int pickColor() {
        return pickColor(0, 0x00FFFFFF);
    }

    // =========================================================================
    // High-Level Shell Integration: Reveal in Explorer & Properties
    // =========================================================================

    /**
     * Opens Windows Explorer and selects/highlights the specified file or directory.
     *
     * @param filePath Absolute path of file or directory.
     * @return true if Explorer was invoked successfully.
     */
    public static boolean showInFolder(String filePath) {
        return nativeShowInFolder(filePath);
    }

    /**
     * Opens the native Windows Properties sheet for a file or folder.
     *
     * @param parentHwnd Parent window handle (or 0).
     * @param filePath   Absolute path of target file or folder.
     * @return true if invoked successfully.
     */
    public static boolean showProperties(long parentHwnd, String filePath) {
        return nativeShowProperties(parentHwnd, filePath);
    }

    public static boolean showProperties(String filePath) {
        return showProperties(0, filePath);
    }

    // =========================================================================
    // High-Level Native Font Chooser
    // =========================================================================

    /**
     * Opens the native Windows Font Chooser (ChooseFontW).
     *
     * @param parentHwnd Parent window handle (or 0).
     * @param initial    Initial font to pre-select (or null for Segoe UI 10pt).
     * @return Selected {@link DialogFont}, or null if cancelled.
     */
    public static DialogFont pickFont(long parentHwnd, DialogFont initial) {
        String name = initial != null ? initial.getFamily() : "Segoe UI";
        int size = initial != null ? initial.getSizePt() : 10;
        boolean bold = initial != null && initial.isBold();
        boolean italic = initial != null && initial.isItalic();

        String raw = nativePickFont(parentHwnd, name, size, bold, italic);
        if (raw == null || raw.isEmpty()) return null;

        String[] parts = raw.split(";");
        if (parts.length >= 4) {
            String fam = parts[0];
            int pt = Integer.parseInt(parts[1]);
            boolean isB = Boolean.parseBoolean(parts[2]);
            boolean isI = Boolean.parseBoolean(parts[3]);
            return new DialogFont(fam, pt, isB, isI);
        }
        return null;
    }

    public static DialogFont pickFont(DialogFont initial) {
        return pickFont(0, initial);
    }

    public static DialogFont pickFont() {
        return pickFont(0, null);
    }

    // =========================================================================
    // Native JNI Declarations
    // =========================================================================

    private static native String nativeOpenFile(long parentHwnd, String title, String defaultFolder, String[] filterDescs, String[] filterExts);

    private static native String[] nativeOpenFiles(long parentHwnd, String title, String defaultFolder, String[] filterDescs, String[] filterExts);

    private static native String nativeSaveFile(long parentHwnd, String title, String defaultFolder, String defaultName, String defaultExt, String[] filterDescs, String[] filterExts);

    private static native String nativeOpenFolder(long parentHwnd, String title, String defaultFolder);

    private static native int nativeShowTaskDialog(long parentHwnd, String title, String instruction, String content, int buttons, int iconId);

    private static native int nativePickColor(long parentHwnd, int initialRgb);

    private static native boolean nativeShowInFolder(String filePath);

    private static native boolean nativeShowProperties(long parentHwnd, String filePath);

    private static native String nativePickFont(long parentHwnd, String name, int sizePt, boolean bold, boolean italic);
}
