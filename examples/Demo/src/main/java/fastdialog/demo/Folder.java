package fastdialog.demo;

import fastdialog.FastDialog;

/**
 * Dedicated Demo: Modern Windows 11 Native Folder Picker Dialog.
 */
public class Folder {
    public static void main(String[] args) {
        System.out.println("=================================================");
        System.out.println("FastDialog 0.1.0 — Windows 11 Folder Picker Demo");
        System.out.println("=================================================");

        if (!FastDialog.isNativeAvailable()) {
            System.err.println("Load error: " + FastDialog.getLoadError());
            return;
        }

        System.out.println("\nOpening Windows 11 Native Folder Picker Dialog...");
        String currentProjectDir = System.getProperty("user.dir");
        String folder = FastDialog.openFolder(
                0,
                "Select a Target Directory",
                currentProjectDir
        );

        if (folder != null) {
            System.out.println("\nSelected Folder: " + folder);
        } else {
            System.out.println("\n<Dialog was cancelled by user>");
        }
    }
}
