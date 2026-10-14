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
    // Native JNI Declarations
    // =========================================================================

    private static native String nativeOpenFile(long parentHwnd, String title, String defaultFolder, String[] filterDescs, String[] filterExts);

    private static native String[] nativeOpenFiles(long parentHwnd, String title, String defaultFolder, String[] filterDescs, String[] filterExts);

    private static native String nativeSaveFile(long parentHwnd, String title, String defaultFolder, String defaultName, String defaultExt, String[] filterDescs, String[] filterExts);

    private static native String nativeOpenFolder(long parentHwnd, String title, String defaultFolder);
}
