package fastdialog.demo;

import fastdialog.FastDialog;

import java.io.File;

/**
 * Dedicated Demo: Windows Shell Integration (Reveal in Explorer & Show Properties).
 */
public class ExplorerOps {
    public static void main(String[] args) {
        System.out.println("=================================================");
        System.out.println("FastDialog 0.1.0 — Windows Explorer Ops Demo");
        System.out.println("=================================================");

        if (!FastDialog.isNativeAvailable()) {
            System.err.println("Load error: " + FastDialog.getLoadError());
            return;
        }

        File targetFile = new File(System.getProperty("user.dir"), "pom.xml");
        String path = targetFile.getAbsolutePath();

        System.out.println("\n[1] Revealing 'pom.xml' in Windows Explorer (selected)...");
        boolean revealed = FastDialog.showInFolder(path);
        System.out.println("Reveal status: " + (revealed ? "SUCCESS (Explorer opened)" : "FAILED"));

        System.out.println("\n[2] Opening Native Windows Properties Sheet for 'pom.xml'...");
        boolean propOpened = FastDialog.showProperties(0, path);
        System.out.println("Properties sheet status: " + (propOpened ? "SUCCESS (Sheet invoked)" : "FAILED"));
    }
}
