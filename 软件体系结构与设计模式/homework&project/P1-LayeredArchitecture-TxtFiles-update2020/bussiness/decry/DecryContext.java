package bussiness.decry;

import dataAcess.MiscUtil;

import java.util.Vector;

public class DecryContext {

    public static Vector doDecryption(){
        Decryption info = new DecryptedInfo1();
        Vector v = info.log();
        info = new DecryptedInfo2();
        v.addAll(info.log());
        info = new DecryptedInfo3();
        v.addAll(info.log());
        v = MiscUtil.removeDuplicates(v);
        return v;
    }
}
