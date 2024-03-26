package main.processor;

import javax.swing.*;
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.regex.Matcher;
import java.util.regex.Pattern;


public class ManProcessor2 implements BatchProcessing{
   private JTextArea resultTxt;
   private BufferedReader input = null;
   private BufferedWriter output1 = null;
   private BufferedWriter output2 = null;
   //String file = "UpdatedFiles/Man25+.txt";

   public ManProcessor2(String inFile, String outFile1, String outFile2, JTextArea resultTxtA)throws IOException{
      input = new BufferedReader(new FileReader(inFile));
	  output1 = new BufferedWriter(new FileWriter(outFile1));
	  output2 = new BufferedWriter(new FileWriter(outFile2));
	  resultTxt=resultTxtA;
   }

   /*---------------------------------------------------------------------*/
   /* This method simply reads input file (Man18+) line by line, and  */
   /* write each line read to one of the two output files, one is
   /* a file containing men whose ages are above 25, and the other
   /* is a file containing men whose ages are between 18 and 25. the data
   /* will not be accumulated in this class. Once a line is read,
   /* it is written immedietely to an output file                      */
    // output1 为大于25岁的文件存储
    // output2 为18~25岁的文件存储
   /*------------------------------------------------------------------*/
   public void processData(){
       try{
           String line = input.readLine();
           while(line != null){ //not end of file
               String[] words = line.split("\\s");

               if (isYoungerThan25 (words)){
                   output2.write(line+"\n");
               }
               else{
                   output1.write(line+"\n");
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
    private boolean isYoungerThan25 (String[] words)   {
        boolean flag = false;
        for(int k=0; k< words.length; k++){  //One line
            if (isDate(words[k])) {
                flag = isYoungerThan25(words[k]);
            }
        }
        return flag;
    }
    private boolean isYoungerThan25 (String word)   {
        boolean flag = false;
        String[] dateParts = word.split("-");
        if (dateParts.length == 3) {
            int year = Integer.parseInt(dateParts[0]);
            if (year > 1989) { // 1996对应18岁，则程序日期为2014年
                flag = true;
            }
        }
        return flag;
    }
    private boolean isDate(String word) {
        // 使用正则表达式匹配日期格式：YYYY-MM-DD
        String datePattern = "^\\d{4}-\\d{2}-\\d{2}$";
        Pattern pattern = Pattern.compile(datePattern);
        Matcher matcher = pattern.matcher(word);
        return matcher.matches();
    }
}