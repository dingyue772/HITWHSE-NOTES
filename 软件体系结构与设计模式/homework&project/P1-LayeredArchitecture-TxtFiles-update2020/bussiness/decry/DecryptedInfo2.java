package bussiness.decry;

import dataAcess.MiscUtil;
import dataAcess.TxtFileReader;

import java.util.Vector;

public class DecryptedInfo2 implements Decryption {
    public static final String GROUPSWAP = "Group-swap";

    @Override
    public String decryptName(String name) {
        char[] nameArr = name.toCharArray();
        char[] evenCharArr = getEvenCharArray();
        char[] oddCharArr = getOddCharArray();
        char[] evenUpperCharArr = getEvenUpperCharArray();
        char[] oddUpperCharArr = getOddUpperCharArray();
        int length = nameArr.length;
        int len = 13;

        for (int i = 0; i < length; i++) {
            for (int j = 0; j < len; j++) {
                if (nameArr[i] == evenCharArr[j])
                    nameArr[i] = oddCharArr[j];
                else if (nameArr[i] == oddCharArr[j])
                    nameArr[i] = evenCharArr[j];
                else if (nameArr[i] == evenUpperCharArr[j])
                    nameArr[i] = oddUpperCharArr[j];
                else if (nameArr[i] == oddUpperCharArr[j])
                    nameArr[i] = evenUpperCharArr[j];
            }
        }
        return new String(nameArr);
    }

    @Override
    public String decryptCode(String code) {
        char[] codeCharArr = code.toCharArray();
        char[] evenCharArr = getEvenCharArray();
        char[] oddCharArr = getOddCharArray();
        char[] evenUpperCharArr = getEvenUpperCharArray();
        char[] oddUpperCharArr = getOddUpperCharArray();
        char[] evenNumArr = getEvenNumArray();
        char[] oddNumArr = getOddNumArray();
        int length = codeCharArr.length;
        int len = 13;
        int numLen = 5;

        if (length != 12) {
            System.out.println("Incorrect code length.");
        } else {
            for (int i = 0; i < length; i++) {
                for (int j = 0; j < len; j++) {
                    if (codeCharArr[i] == evenCharArr[j])
                        codeCharArr[i] = oddCharArr[j];
                    else if (codeCharArr[i] == oddCharArr[j])
                        codeCharArr[i] = evenCharArr[j];
                    else if (codeCharArr[i] == evenUpperCharArr[j])
                        codeCharArr[i] = oddUpperCharArr[j];
                    else if (codeCharArr[i] == oddUpperCharArr[j])
                        codeCharArr[i] = evenUpperCharArr[j];
                }
                for (int j = 0; j < numLen; j++) {
                    if (codeCharArr[i] == evenNumArr[j])
                        codeCharArr[i] = oddNumArr[j];
                    else if (codeCharArr[i] == oddNumArr[j])
                        codeCharArr[i] = evenNumArr[j];
                }
            }
        }
        return new String(codeCharArr);
    }

    @Override
    public Vector<String> log() {
        String filePath = GROUPSWAP + ".txt";
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

    private char[] getEvenCharArray() {
        String str = "acegikmoqsuwy";
        char[] chArray = str.toCharArray();
        return chArray;
    }

    private char[] getOddCharArray() {
        String str = "bdfhjlnprtvxz";
        char[] chArray = str.toCharArray();
        return chArray;
    }

    private char[] getEvenUpperCharArray() {
        String str = "ACEGIKMOQSUWY";
        char[] chArray = str.toCharArray();
        return chArray;
    }

    private char[] getOddUpperCharArray() {
        String str = "BDFHJLNPRTVXZ";
        char[] chArray = str.toCharArray();
        return chArray;
    }

    private char[] getEvenNumArray() {
        String str = "02468";
        char[] chArray = str.toCharArray();
        return chArray;
    }

    private char[] getOddNumArray() {
        String str = "13579";
        char[] chArray = str.toCharArray();
        return chArray;
    }
}
