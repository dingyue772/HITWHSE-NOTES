package bussiness.decry;

import javax.swing.*;
import java.util.Vector;

public interface Decryption {
    public abstract String decryptName(String inputStr);
    public abstract String decryptCode (String code);
    public abstract Vector log();
}
