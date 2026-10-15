package fastdialog.demo;

import fastdialog.DialogFilter;
import fastdialog.FastDialog;

import java.util.Arrays;

/**
 * Dedicated Demo: Modern Windows 11 Multi-File Selection Dialog.
 */
public class MultiFile {
    public static void main(String[] args) {
        System.out.println("=================================================");
        System.out.println("FastDialog 0.1.0 — Windows 11 Multi-File Demo");
        System.out.println("=================================================");

        if (!FastDialog.isNativeAvailable()) {
            System.err.println("Load error: " + FastDialog.getLoadError());
            return;
        }

        System.out.println("\nOpening Windows 11 Multi-File Selection Dialog...");
        String currentProjectDir = System.getProperty("user.dir");
        String[] files = FastDialog.openFiles(
                0,
                "Select Multiple Files (Ctrl/Shift + Click)",
                currentProjectDir,
                new DialogFilter("Source Files (*.java;*.kt;*.txt;*.md;*.xml)", "*.java;*.kt;*.txt;*.md;*.xml"),
                new DialogFilter("All Files (*.*)", "*.*")
        );

        if (files != null) {
            System.out.println("\nSelected (" + files.length + " files):");
            for (int i = 0; i < files.length; i++) {
                System.out.println("  [" + (i + 1) + "] " + files[i]);
            }
        } else {
            System.out.println("\n<Dialog was cancelled by user>");
        }
    }
}
