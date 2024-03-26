package main.info;

import main.ClientGUI;
import main.writer.MessageWriter;

public class EncryptedInfo3 extends AgentInfo{
    private MessageWriter writer;
    public EncryptedInfo3(MessageWriter l){
        writer = l;
    }
    @Override
    public void log(String lastNm, String firstNm, String code){
        if( isValidAgentName(lastNm) == false ||  isValidAgentName(firstNm) == false    ){
            System.out.println("Invalid agent name. Only English characters are allowed.");
            System.exit(1);
        }
        if( isValidCode(code) == false){
            System.out.println("Invalid agent code. Only English characters and numbers are allowed.");
            System.exit(1);
        }

        String fName = encryptName(firstNm);
        ClientGUI.resultTxt.append("Encrypted agent first name: "+ fName +"\n");
        String lName = encryptName(lastNm);
        ClientGUI.resultTxt.append("Encrypted agent last name: "+ lName +"\n");

        String codeStr = encryptCode(code);
        ClientGUI.resultTxt.append("Encrypted agent code: "+ codeStr +"\n\n");

        try{
            writer.logMsg(lName, fName, codeStr);
        }
        catch (Exception e) {
            e.printStackTrace();
        }

    }

    //This method can be used to encrypt the first name or the last name
    private String encryptName(String msg) {
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
    private String encryptCode (String code) {
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
