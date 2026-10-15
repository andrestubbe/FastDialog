package fastdialog.demo;

import fastdialog.FastDialog;

public class AboutDemo {
    public static void main(String[] args) {
        System.out.println("FastDialog ShellAbout Demo");
        System.out.println("---------------------------");
        System.out.println("Opening native Windows About dialog...");

        boolean ok = FastDialog.showAbout(
            "FastJava Suite#FastDialog v0.1.0",
            "High-performance native Win32/COM dialog integration for Java 21+.\n" +
            "Developed by Andre Stubbe (andrestubbe)\n" +
            "Built on Windows 11 Shell API."
        );

        System.out.println("Dialog closed. Result: " + ok);
    }
}
