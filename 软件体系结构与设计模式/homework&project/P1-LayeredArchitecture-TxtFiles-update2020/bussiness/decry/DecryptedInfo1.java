package bussiness.decry;

import dataAcess.MiscUtil;
import dataAcess.TxtFileReader;

import java.util.Vector;

public class DecryptedInfo1 implements Decryption{
    public static final String FOLDING = "Folding";
    @Override
    public String decryptName(String inputStr) {
        char[] alphabet = getAlphabetArray();
        char[] chars = inputStr.toCharArray();

        for (int m = 0; m < chars.length; m++) {
            for (int n = 0; n < 26; n++) {
                if (chars[m] == alphabet[n]) {
                    chars[m] = alphabet[25 - n];
                    break;
                } else if (chars[m] == Character.toUpperCase(alphabet[n])) {
                    chars[m] = Character.toUpperCase(alphabet[25 - n]);
                    break;
                }
            }
        }

        return new String(chars);
    }

    @Override
    public String decryptCode(String code) {
        char[] digitArr = getDigitArray();
        char[] codeChars = code.toCharArray();
        char[] alphabet = getAlphabetArray();

        if (codeChars.length != 12) {
            System.out.println("Incorrect code length.");
        } else {
            for (int m = 0; m < codeChars.length; m++) {
                for (int n = 0; n < 26; n++) {
                    if (codeChars[m] == alphabet[n]) {
                        codeChars[m] = alphabet[25 - n];
                        break;
                    } else if (codeChars[m] == Character.toUpperCase(alphabet[n])) {
                        codeChars[m] = Character.toUpperCase(alphabet[25 - n]);
                        break;
                    }
                }

                for (int j = 0; j < 10; j++) {
                    if (codeChars[m] == digitArr[j]) {
                        codeChars[m] = digitArr[9 - j];
                        break;
                    }
                }
            }
        }

        return new String(codeChars);
    }
    @Override
    public Vector<String> log() {
        String filePath = FOLDING + ".txt";
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
    private char[] getAlphabetArray() {
        String str = "abcdefghijklmnopqrstuvwxyz";
        return str.toCharArray();
    }

    private char[] getDigitArray() {
        String str = "0123456789";
        return str.toCharArray();
    }
}