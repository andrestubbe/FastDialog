package fastdialog.demo;

import fastdialog.FastDialog;

public class CommandLinksDemo {
    public static void main(String[] args) {
        System.out.println("FastDialog Command Links & Custom TaskDialog Demo");
        System.out.println("-------------------------------------------------");

        String[] options = new String[] {
            "Save and Continue\nSave all modified documents and advance to next phase.",
            "Export as ZIP Archive\nPackage workspace contents into a single portable file.",
            "Discard All Changes\nReset working tree to initial state (destructive)."
        };

        String details = "Stack trace / Log info:\n" +
                         "FastDialog initialized in 1.4ms\n" +
                         "Direct Win32 TaskDialogIndirect invoked with TDF_USE_COMMAND_LINKS\n" +
                         "Windows 11 Visual Styles enabled.";

        FastDialog.TaskDialogResponse resp = FastDialog.showTaskDialog(
            0,
            "FastJava Project Manager",
            "What would you like to do next?",
            "Please select an action from the options below:",
            details,
            "Don't show this question again",
            options,
            FastDialog.ICON_SHIELD
        );

        System.out.println("Pressed Button ID: " + resp.getButtonId());
        if (resp.getButtonId() >= 100 && resp.getButtonId() < 100 + options.length) {
            System.out.println("Selected Option : " + options[resp.getButtonId() - 100].split("\n")[0]);
        }
        System.out.println("Checkbox Checked  : " + resp.isVerificationChecked());
    }
}
