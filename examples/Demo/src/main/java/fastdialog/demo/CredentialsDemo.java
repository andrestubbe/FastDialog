package fastdialog.demo;

import fastdialog.DialogCredentials;
import fastdialog.FastDialog;

public class CredentialsDemo {
    public static void main(String[] args) {
        System.out.println("FastDialog Security Credentials Demo");
        System.out.println("------------------------------------");
        System.out.println("Opening Windows Security / Windows Hello credential dialog...");

        DialogCredentials creds = FastDialog.promptCredentials(
            "FastJava Admin Access",
            "Please confirm your Windows credentials or PIN to continue."
        );

        if (creds != null) {
            System.out.println("Credentials entered successfully!");
            System.out.println("User: " + creds.getUsername());
            System.out.println("Password length: " + creds.getPassword().length() + " chars");
        } else {
            System.out.println("Credential prompt was cancelled.");
        }
    }
}
