package fastdialog.demo;

import fastdialog.FastDialog;

/**
 * Dedicated Demo: Windows Native Color Chooser (ChooseColorW).
 */
public class ColorPicker {
    public static void main(String[] args) {
        System.out.println("=================================================");
        System.out.println("FastDialog 0.1.0 — Windows 11 Color Picker Demo");
        System.out.println("=================================================");

        if (!FastDialog.isNativeAvailable()) {
            System.err.println("Load error: " + FastDialog.getLoadError());
            return;
        }

        System.out.println("\nOpening Windows Native Color Chooser (Default Cyan: 0x00E0FF)...");
        int pickedColor = FastDialog.pickColor(0, 0x00E0FF);

        if (pickedColor != -1) {
            int a = (pickedColor >> 24) & 0xFF;
            int r = (pickedColor >> 16) & 0xFF;
            int g = (pickedColor >> 8) & 0xFF;
            int b = pickedColor & 0xFF;
            System.out.printf("\nSelected 32-bit ARGB: 0x%08X (R:%d, G:%d, B:%d, Alpha:%d)\n", pickedColor, r, g, b, a);
            System.out.println("Perfect for immediate use with FastTheme.get() or AWT Color!");
        } else {
            System.out.println("\n<Color picker was cancelled by user>");
        }
    }
}
