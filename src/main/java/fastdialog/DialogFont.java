package fastdialog;

/**
 * Encapsulates font attributes chosen from the native Windows Font Dialog.
 */
public final class DialogFont {
    private final String family;
    private final int sizePt;
    private final boolean bold;
    private final boolean italic;

    public DialogFont(String family, int sizePt, boolean bold, boolean italic) {
        this.family = family != null ? family : "Segoe UI";
        this.sizePt = sizePt > 0 ? sizePt : 10;
        this.bold = bold;
        this.italic = italic;
    }

    public String getFamily() {
        return family;
    }

    public int getSizePt() {
        return sizePt;
    }

    public boolean isBold() {
        return bold;
    }

    public boolean isItalic() {
        return italic;
    }

    @Override
    public String toString() {
        StringBuilder sb = new StringBuilder(family);
        sb.append(" ").append(sizePt).append("pt");
        if (bold && italic) sb.append(" [Bold Italic]");
        else if (bold) sb.append(" [Bold]");
        else if (italic) sb.append(" [Italic]");
        return sb.toString();
    }
}
