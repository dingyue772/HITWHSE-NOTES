package main.processor;

import javax.swing.*;
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.regex.Matcher;
import java.util.regex.Pattern;


public class WomanProcessor1 implements BatchProcessing{
   private JTextArea resultTxt;
   private BufferedReader input = null;
   private BufferedWriter output1 = null;
   private BufferedWriter output2 = null;


   public WomanProcessor1(String inFile, String outFile1, String outFile2, JTextArea resultTxtA)throws IOException{
      input = new BufferedReader(new FileReader(inFile));
	  output1 = new BufferedWriter(new FileWriter(outFile1));
	  output2 = new BufferedWriter(new FileWriter(outFile2));
	  resultTxt=resultTxtA;
   }

   /*-------------------------------------------------------------*/
   /* This method simply reads input file (Woman) line by line, and  */
   /* write each line read to one of the two output files, one is
   /* a file containing women whose age are under 18, and the other
   /* is a file containing women whose ages are above 18. the data
   /* will not be accumulated in this class. Once a line is read,
   /* it is written immedietely to an output file                 */
	// output1 为小于18岁的文件存储
	// output2 为大于18岁的文件存储
   /*-------------------------------------------------------------*/
   public void processData(){
      try{
         String line = input.readLine();
         while(line != null){ //not end of file
            String[] words = line.split("\\s");

             if (isYoungerThan18 (words)){
			     output1.write(line+"\n");
		     }
		     else{
			     output2.write(line+"\n");
			     resultTxt.append(line+"\n");
		     }
            line = input.readLine();
         }

         if (input != null){
		    input.close();
		 }
		 if (output1 != null){
		    output1.close();
         }
         if (output2 != null){
		    output2.close();
         }
      }
      catch(IOException exc){
         exc.printStackTrace();
         System.err.println("Error: failed Woman processor 1");
         System.exit(1);
      }
   }
	 private boolean isYoungerThan18 (String[] words)   {
		 boolean flag = false;
		 for(int k=0; k< words.length; k++){  //One line
			 if (isDate(words[k])) {
				 flag = isYoungerThan18(words[k]);
			 }
		 }
		return flag;
  	}
	 private boolean isYoungerThan18 (String word)   {
		 String[] dateParts = word.split("-");
		 if (dateParts.length == 3) {
			 int year = Integer.parseInt(dateParts[0]);
			 if (year > 1996) { // 1996对应18岁，则程序日期为2014年
			 	return true;
			 }
			 else
			 	return false;
		 }
		 return false;
 	}
	private boolean isDate(String word) {
		// 使用正则表达式匹配日期格式：YYYY-MM-DD
		String datePattern = "^\\d{4}-\\d{2}-\\d{2}$";
		Pattern pattern = Pattern.compile(datePattern);
		Matcher matcher = pattern.matcher(word);
		return matcher.matches();
	}
}


