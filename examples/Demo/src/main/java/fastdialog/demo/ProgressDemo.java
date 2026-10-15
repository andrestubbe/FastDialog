package fastdialog.demo;

import fastdialog.FastDialog;
import fastdialog.NativeProgress;

public class ProgressDemo {
    public static void main(String[] args) throws InterruptedException {
        System.out.println("FastDialog Native IProgressDialog Demo");
        System.out.println("-------------------------------------");
        System.out.println("Spawning native Windows asynchronous progress dialog...");

        try (NativeProgress progress = FastDialog.createProgress("Syncing FastJava Repository", "Preparing files...")) {
            int total = 100;
            for (int i = 0; i <= total; i++) {
                if (progress.isCancelled()) {
                    System.out.println("User clicked Cancel in the native dialog!");
                    break;
                }

                String line2 = "Processing item " + i + " of " + total;
                String line3 = (100 - i) + " seconds remaining...";
                progress.update(i, total, line2, line3);

                Thread.sleep(60);
            }
            System.out.println("Progress completed.");
        }
    }
}
