package main.writer;/*------------------------------------------------*/
/* THis class implements interface MessageLogger  */
/* This class provides a method called            */
/*            logMsg(String msg)                  */
/* to write a message into a file called log.txt  */
/*------------------------------------------------*/


public class TxtFileWriter implements MessageWriter {

  public void logMsg(String lastNm, String firstNm, String code) throws Exception{
    FileUtil futil = new FileUtil();
    futil.writeToFile("main/writer/Encrypted.txt",lastNm + " " + firstNm + " " +code+ " " + "\r\n", true, true); // "\r\n"代表写入新行
    System.out.println("加密内容已写入TxtFile");
  }
}
