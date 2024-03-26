package bussiness.encry;

import dataAcess.TxtFileWriter;

import javax.swing.*;

public class EncryptedInfo3 implements Encryption{
    public static final String CAESAR = "Caesar";
    private TxtFileWriter writer = new TxtFileWriter();
    @Override
    public void log(String lastNm, String firstNm, String code, JTextArea txt){
        String lName = encryptName(lastNm);
        String fName = encryptName(firstNm);
        String codeStr = encryptCode(code);
        txt.append("Encrypted agent first name: "+ fName +"\n");
        txt.append("Encrypted agent last name: "+ lName +"\n");
        txt.append("Encrypted agent code: "+ codeStr +"\n\n");

        writer.logMsg(CAESAR, lName, fName, codeStr);
        txt.append("Data has been saved into file "+ CAESAR +".txt"+"\n\n");

    }

    //This method can be used to encrypt the first name or the last name
    public String encryptName(String msg) {
        char[] chars = msg.toCharArray();
        for (int m=0; m<chars.length; m++){
            if (Character.isUpperCase(chars[m])){
                chars[m] = (char)((chars[m]-'A'+1)%26 + 'A');
            }
            else
                chars[m] = (char)((chars[m]-'a'+1)%26 + 'a');
        }
        return new String(chars);
    }

    //This method can be used to encrypt the agent code
    public String encryptCode (String code) {
        char[] chars = code.toCharArray();
        for (int m=0; m<chars.length; m++){
            if (Character.isDigit((chars[m])))
                chars[m] = (char)((chars[m]-'0'+1)%10+'0');
            else if (Character.isUpperCase(chars[m])){
                chars[m] = (char)((chars[m]-'A'+1)%26 + 'A');
            }
            else
                chars[m] = (char)((chars[m]-'a'+1)%26 + 'a');
        }
        return new String(chars);
    }

}
