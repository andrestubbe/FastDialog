package fastdialog.demo;

import fastdialog.FastDialog;

/**
 * Dedicated Demo: Modern Windows 10/11 TaskDialog.
 */
public class TaskDialogDemo {
    public static void main(String[] args) {
        System.out.println("=================================================");
        System.out.println("FastDialog 0.1.0 — Windows 11 TaskDialog Demo");
        System.out.println("=================================================");

        if (!FastDialog.isNativeAvailable()) {
            System.err.println("Load error: " + FastDialog.getLoadError());
            return;
        }

        System.out.println("\n[1] Showing Information TaskDialog...");
        FastDialog.showInfo(
                "FastJava Deployment",
                "Application Core Initialized Successfully",
                "The native DWM bridge and Shell dialog subsystem are active with zero heap allocation."
        );

        System.out.println("\n[2] Showing Confirmation TaskDialog (Yes / No)...");
        boolean confirmed = FastDialog.showConfirm(
                0,
                "Security Confirmation",
                "Do you want to grant hardware access?",
                "This allows low-latency direct memory mapping and display telemetry sensing."
        );
        System.out.println("User Choice: " + (confirmed ? "YES" : "NO"));
    }
}
