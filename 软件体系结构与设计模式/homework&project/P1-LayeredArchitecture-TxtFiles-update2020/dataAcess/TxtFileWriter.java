package dataAcess;
/*------------------------------------------------*/
/* THis class implements interface MessageLogger  */
/* This class provides a method called            */
/*            logMsg(String msg)                  */
/* to write a message into a file called log.txt  */
/*------------------------------------------------*/

public class TxtFileWriter {

	public void  logMsg(String encryptOption, String lastNm, String firstNm, String code) {
    	FileUtil futil = new FileUtil();
    	futil.writeToFile(encryptOption+".txt",lastNm + " " + firstNm + " " +code+ " " + "\r\n", true, true); // "\r\n"代表写入新行
  }
}
