package dataAcess;

import java.util.Vector;

public class TxtFileReader {
    public static Vector<String> logMsg(String encryptOption){
        FileUtil futil = new FileUtil();
        return futil.fileToVector(encryptOption);
    }
}
