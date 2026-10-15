package fastdialog;

/**
 * Native Windows Progress Dialog (IProgressDialog).
 * Displays a system progress bar, status text, and cancel button running on a native background thread.
 */
public class NativeProgress implements AutoCloseable {
    private final long handle;

    NativeProgress(long handle) {
        this.handle = handle;
    }

    /**
     * Updates the progress bar and current status lines.
     *
     * @param current Current value (e.g. 45).
     * @param total   Maximum value (e.g. 100).
     * @param line2   Text shown directly above progress bar.
     * @param line3   Optional subtext (e.g. "Time remaining...").
     */
    public void update(int current, int total, String line2, String line3) {
        if (handle != 0) {
            FastDialog.nativeUpdateProgress(handle, current, total, line2, line3);
        }
    }

    /**
     * Checks if the user clicked the native "Cancel" button.
     *
     * @return True if cancel was clicked.
     */
    public boolean isCancelled() {
        if (handle == 0) return true;
        return FastDialog.nativeIsProgressCancelled(handle);
    }

    /**
     * Closes and disposes the native progress dialog.
     */
    @Override
    public void close() {
        if (handle != 0) {
            FastDialog.nativeCloseProgress(handle);
        }
    }
}
