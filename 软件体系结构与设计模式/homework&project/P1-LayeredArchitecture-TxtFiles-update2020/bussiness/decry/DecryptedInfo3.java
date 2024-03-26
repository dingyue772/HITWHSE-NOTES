package bussiness.decry;

import dataAcess.MiscUtil;
import dataAcess.TxtFileReader;

import java.util.Vector;

public class DecryptedInfo3 implements Decryption {
    public static final String CAESAR = "Caesar";

    @Override
    public String decryptName(String msg) {
        char[] chars = msg.toCharArray();
        for (int m = 0; m < chars.length; m++) {
            if (Character.isUpperCase(chars[m])) {
                chars[m] = (char) ((chars[m] - 'A' + 25) % 26 + 'A');
            } else {
                chars[m] = (char) ((chars[m] - 'a' + 25) % 26 + 'a');
            }
        }
        return new String(chars);
    }

    @Override
    public String decryptCode(String code) {
        char[] chars = code.toCharArray();
        for (int m = 0; m < chars.length; m++) {
            if (Character.isDigit((chars[m])))
                chars[m] = (char) ((chars[m] - '0' + 9) % 10 + '0');
            else if (Character.isUpperCase(chars[m])) {
                chars[m] = (char) ((chars[m] - 'A' + 25) % 26 + 'A');
            } else {
                chars[m] = (char) ((chars[m] - 'a' + 25) % 26 + 'a');
            }
        }
        return new String(chars);
    }

    @Override
    public Vector<String> log() {
        String filePath = CAESAR + ".txt";
        Vector<String> encryptedLines = TxtFileReader.logMsg(filePath);
        encryptedLines = MiscUtil.removeDuplicates(encryptedLines);
        Vector<String> decryptedLines = new Vector<>();

        for (String encryptedLine : encryptedLines) {
            // Split the line into firstName, lastName, and code
            String[] parts = encryptedLine.split("\\s+");
            if (parts.length == 3) {
                String encryptedFirstName = parts[0];
                String encryptedLastName = parts[1];
                String encryptedCode = parts[2];

                // Decrypt each part
                String decryptedFirstName = decryptName(encryptedFirstName);
                String decryptedLastName = decryptName(encryptedLastName);
                String decryptedCode = decryptCode(encryptedCode);

                // Combine decrypted parts into a new line
                String decryptedLine = decryptedFirstName + " " + decryptedLastName + " " + decryptedCode;

                // Add the decrypted line to the Vector
                decryptedLines.add(decryptedLine);
            }
        }

        return decryptedLines;
    }
}
