package fastdialog.demo;

import fastdialog.DialogFilter;
import fastdialog.FastDialog;

/**
 * Dedicated Demo: Modern Windows 11 Native Save File Dialog.
 */
public class Save {
    public static void main(String[] args) {
        System.out.println("=================================================");
        System.out.println("FastDialog 0.1.0 — Windows 11 Save File Demo");
        System.out.println("=================================================");

        if (!FastDialog.isNativeAvailable()) {
            System.err.println("Load error: " + FastDialog.getLoadError());
            return;
        }

        System.out.println("\nOpening Windows 11 Native Save File Dialog...");
        String currentProjectDir = System.getProperty("user.dir");
        String savePath = FastDialog.saveFile(
                0,
                "Save Generated Document",
                currentProjectDir,
                "document.json",
                "json",
                new DialogFilter("JSON Files (*.json)", "*.json"),
                new DialogFilter("Text Files (*.txt)", "*.txt"),
                new DialogFilter("All Files (*.*)", "*.*")
        );

        if (savePath != null) {
            System.out.println("\nSave Target: " + savePath);
        } else {
            System.out.println("\n<Dialog was cancelled by user>");
        }
    }
}
