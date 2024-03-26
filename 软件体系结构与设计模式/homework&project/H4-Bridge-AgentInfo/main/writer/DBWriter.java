package main.writer;/*------------------------------------------------*/
/* THis class implements interface MessageLogger  */
/* This class provides a method called            */
/*            logMsg(String msg)                  */
/* to write a message into a file called log.txt  */
/*------------------------------------------------*/

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;

public class DBWriter implements MessageWriter {
   private String lastName;
   private String firstName;
   private String codeNum;

   private static final String DBDRIVER = "com.mysql.cj.jdbc.Driver";
   private static final String DBCONNSTR = "jdbc:mysql://localhost:3306/test";
   private static final String USER = "root";
   private static final String PWORD = "1234";

  public void logMsg(String lastNm, String firstNm, String code) throws Exception  {
	 firstName = firstNm;
	 lastName = lastNm;
	 codeNum = code;
	 Class.forName(DBDRIVER);
	 String sql = "INSERT INTO Agent (last_name, first_name, code_num)" + "values(?, ?, ?)";
	 try (Connection connection = DriverManager.getConnection(DBCONNSTR, USER, PWORD);
          PreparedStatement preparedStatement = connection.prepareStatement(sql)) {
         preparedStatement.setString(1, lastName);
         preparedStatement.setString(2, firstName);
         preparedStatement.setString(3, codeNum);

         preparedStatement.executeUpdate();

         System.out.println("加密内容已写入MySQL数据库");
     }
  }
}
