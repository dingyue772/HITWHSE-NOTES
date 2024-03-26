package main;

import main.filter.*;

import java.io.FileInputStream;
import java.io.IOException;
import javax.swing.*;


public class AssemblyOfPipeLines {
    // 在GUI中的调用语句：apline.assembly(selectedFile, originalTxt, resultTxt);
   public void assembly(String inFile, JTextArea orig, JTextArea destn) throws IOException {

	     try{
            //Create 4 pipe objects
            Pipe p1 = new Pipe();
      	    Pipe p2 = new Pipe();
      	    Pipe p3 = new Pipe();
      	    Pipe p4 = new Pipe();

      	    //Input from source stream
             // main.PipeFilterTestGui.ORIGINALFILES=“OriginalFiles/”
      	    FileInputStream in = new FileInputStream(PipeFilterTestGui.ORIGINALFILES + inFile);

            //Create 5 filter objects
            Filter input = new InFilter(in, p1,orig);
            Filter decrypt1 = new DecrFilter3(p1, p2);
            Filter decrypt2= new DecrFilter2(p2, p3 );
            Filter decrypt3= new DecrFilter1(p3, p4 );

            //main.filter.Filter output = new main.filter.OutFilter(p4, destn,inFile);
            Filter output = new OutFilter(p4, destn,inFile);

            //Start all the filter threads
            input.start();
            decrypt1.start();
            decrypt2.start();
            decrypt3.start();
            output.start();
         }
         catch(IOException exc) {
            exc.printStackTrace();
         }
   }
}
