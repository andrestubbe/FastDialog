package fastdialog.demo;

import fastdialog.DialogFilter;
import fastdialog.FastDialog;

import java.util.Arrays;

/**
 * Interactive Demo demonstrating Windows 11 Open File, Open Files, Save File, and Folder Picker dialogs.
 */
public class Demo {
    public static void main(String[] args) {
        System.out.println("=================================================");
        System.out.println("FastDialog 0.1.0 — Windows 11 Native Dialog Demo");
        System.out.println("=================================================");
        System.out.println("Native DLL Available: " + FastDialog.isNativeAvailable());

        if (!FastDialog.isNativeAvailable()) {
            System.err.println("Load error: " + FastDialog.getLoadError());
            return;
        }

        System.out.println("\n[1] Opening Single File Dialog (Images / All Files)...");
        String file = FastDialog.openFile(
                0,
                "Select an Image File",
                System.getProperty("user.home"),
                new DialogFilter("Image Files (*.png;*.jpg;*.jpeg;*.webp)", "*.png;*.jpg;*.jpeg;*.webp"),
                new DialogFilter("Text & Source Files (*.txt;*.java;*.md)", "*.txt;*.java;*.md"),
                new DialogFilter("All Files (*.*)", "*.*")
        );
        System.out.println("Result: " + (file != null ? file : "<Cancelled>"));

        System.out.println("\n[2] Opening Multiple Files Dialog (Java / Source Files)...");
        String[] files = FastDialog.openFiles(
                0,
                "Select Multiple Files",
                System.getProperty("user.home"),
                new DialogFilter("Java Files (*.java)", "*.java"),
                new DialogFilter("All Files (*.*)", "*.*")
        );
        System.out.println("Result: " + (files != null ? Arrays.toString(files) : "<Cancelled>"));

        System.out.println("\n[3] Opening Folder Picker Dialog...");
        String folder = FastDialog.openFolder(
                0,
                "Choose a Project Folder",
                System.getProperty("user.dir")
        );
        System.out.println("Result: " + (folder != null ? folder : "<Cancelled>"));

        System.out.println("\n[4] Opening Save File Dialog...");
        String saveTarget = FastDialog.saveFile(
                0,
                "Save Output Report",
                System.getProperty("user.home"),
                "export.json",
                "json",
                new DialogFilter("JSON Files (*.json)", "*.json"),
                new DialogFilter("All Files (*.*)", "*.*")
        );
        System.out.println("Result: " + (saveTarget != null ? saveTarget : "<Cancelled>"));

        System.out.println("\nDemo completed successfully!");
    }
}
