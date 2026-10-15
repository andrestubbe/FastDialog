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

        System.out.println("\nOpening Windows 11 Native Open File Dialog...");
        String file = FastDialog.openFile(
                0,
                "Select a File",
                System.getProperty("user.home"),
                new DialogFilter("Source Files (*.java;*.kt;*.txt;*.md)", "*.java;*.kt;*.txt;*.md"),
                new DialogFilter("Image Files (*.png;*.jpg;*.jpeg;*.webp)", "*.png;*.jpg;*.jpeg;*.webp"),
                new DialogFilter("All Files (*.*)", "*.*")
        );

        if (file != null) {
            System.out.println("Selected: " + file);
        } else {
            System.out.println("<Dialog was cancelled by user>");
        }
    }
}
