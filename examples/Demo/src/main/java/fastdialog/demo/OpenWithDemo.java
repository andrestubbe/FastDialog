package fastdialog.demo;

import fastdialog.FastDialog;
import java.io.File;

public class OpenWithDemo {
    public static void main(String[] args) {
        System.out.println("FastDialog OpenWith Demo");
        System.out.println("------------------------");

        // Use README.md as target
        File target = new File("README.md").getAbsoluteFile();
        System.out.println("Prompting Windows Open With dialog for: " + target);

        boolean ok = FastDialog.openWith(target.getAbsolutePath());
        System.out.println("Dialog executed: " + ok);
    }
}
