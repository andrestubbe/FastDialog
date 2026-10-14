package fastdialog;

/**
 * Filter specification for FastDialog file dialogs.
 */
public final class DialogFilter {
    private final String description;
    private final String extensionPattern;

    public DialogFilter(String description, String extensionPattern) {
        this.description = description != null ? description : "All Files";
        this.extensionPattern = extensionPattern != null ? extensionPattern : "*.*";
    }

    public String getDescription() {
        return description;
    }

    public String getExtensionPattern() {
        return extensionPattern;
    }

    @Override
    public String toString() {
        return description + " (" + extensionPattern + ")";
    }
}
