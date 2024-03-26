import java.io.IOException;


public class EncrFilter2 extends Filter{

   public EncrFilter2(Pipe input, Pipe output){
      super(input, output);
   }

   /*-------------------------------------------------------
   /* Read from the input pipe, character by character.
   /* Encrypt the chars by using Encryption Algorithm 2 (·Ö×é»¥»»Ëã·¨).
   /* Precondition: a text file to be encrypted contains only
   /* English characters and digital numbers. The algorithm is:
   /*       a<-->b, c<-->d, e<-->f, ,..., y<-->z
   /*       0<-->1, 2<-->3, ...8<-->9
   /* Upper case letters are also encrypted the same way
   /* (Upper case letters are encrypted into Upper case letters) .
   /* The encrypted chars are written to the sink pipe char by char
   /*------------------------------------------------------*/
   protected void processData(){

      try{
         int c = inPipe.read(); // Read a character from the source pipe

         while(c != -1){ //not end of file
            char f = (char)c;
			char e = encrypt(f); // to encrypt a character
			outPipe.write(e);  // Write character 'e' to the sink pipe
            c = inPipe.read();
         }
         outPipe.closePipedOutputStream();
         inPipe.closePipedInputStream();
      }
      catch(IOException exc){
         exc.printStackTrace();
         System.err.println("Error: failed encryption 2");
         System.exit(1);
      }
   }

   private char encrypt(char c) {
      char[] lowerCaseChars = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
      char[] upperCaseChars = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
      char[] digits = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

      for (int i = 0; i < lowerCaseChars.length; i++) {
         if (c == lowerCaseChars[i]) {
            return (i % 2 == 0) ? lowerCaseChars[i + 1] : lowerCaseChars[i - 1];
         }
      }

      for (int i = 0; i < upperCaseChars.length; i++) {
         if (c == upperCaseChars[i]) {
            return (i % 2 == 0) ? upperCaseChars[i + 1] : upperCaseChars[i - 1];
         }
      }

      for (int i = 0; i < digits.length; i++) {
         if (c == digits[i]) {
            return (i % 2 == 0) ? digits[i + 1] : digits[i - 1];
         }
      }

      return c;
   }
}


