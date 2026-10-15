package fastdialog.demo;

import fastdialog.DialogFont;
import fastdialog.FastDialog;

/**
 * Dedicated Demo: Windows Native Font Chooser (ChooseFontW).
 */
public class FontPicker {
    public static void main(String[] args) {
        System.out.println("=================================================");
        System.out.println("FastDialog 0.1.0 — Windows 11 Font Picker Demo");
        System.out.println("=================================================");

        if (!FastDialog.isNativeAvailable()) {
            System.err.println("Load error: " + FastDialog.getLoadError());
            return;
        }

        System.out.println("\nOpening Windows Native Font Chooser (Default: Segoe UI 11pt Bold)...");
        DialogFont picked = FastDialog.pickFont(0, new DialogFont("Segoe UI", 11, true, false));

        if (picked != null) {
            System.out.println("\nSelected Font: " + picked);
            System.out.println("  Family: " + picked.getFamily());
            System.out.println("  Size  : " + picked.getSizePt() + " pt");
            System.out.println("  Bold  : " + picked.isBold());
            System.out.println("  Italic: " + picked.isItalic());
        } else {
            System.out.println("\n<Font picker was cancelled by user>");
        }
    }
}
