
/*---------------------------------------------------------*/
/* This adapter class extends InfoValidation and           */
/* implements CusInfoValidator, and so the first           */
/* 4 functionalities listed in class CusInfoValidator      */
/* are automatically inherited from class InfoValidation,  */
/* and in this addapter class, isValidSSNNum(String SSNNum)*/
/* is emplemented.                                         */
/*---------------------------------------------------------*/


class InformationAdapter extends InfoValidation implements CusInfoValidator{

    /*------------------------------------------*/
	/* The Social Security number is a 9-digit  */
	/* number in the format "AAA-GG-SSSS".      */
	/*------------------------------------------*/
	public boolean isValidSSNNum(String SSNNum){
	   boolean isValid=true;
	   if(SSNNum.length() == 0){
		      isValid = false;
		      System.out.println("Empty string ***");
	   }
	   else{
			   String ns = SSNNum.trim();
			   String nStr = ns.replaceAll("\\s{1,}", "");
			   int len = nStr.length();

			   if ( (len==11) && (nStr.charAt(3) == '-') && (nStr.charAt(6) == '-')  ) {
				  for(int m=0; m<len; m++){
					    if(  (m != 3) && (m !=6) && ( Character.isDigit(nStr.charAt(m))==false) ){
					    	isValid=false;
					    }
				  }
			   }
			   else{
				  isValid=false;
			   }
	   }
	   return  isValid;
    }

	public boolean isValidEmailAddr(String emailNum) {
		if (emailNum == null || emailNum.length() < 5) {
			return false;
		}
		int atCount = 0;
		int dotCount = 0;
		char firstChar = emailNum.charAt(0);
		// 若首字符不是字母直接返回false
		if (!Character.isLetter(firstChar))
			return false;

		for (int i = 0; i < emailNum.length(); i++) {
			char currentChar = emailNum.charAt(i);

			if (Character.isLetter(currentChar)) {
				continue;
			} else if (Character.isDigit(currentChar)) {
				continue;
			} else if (currentChar == '@') { // 计数@字符
				atCount++;
			} else if (currentChar == '.') { // 计数.字符
				dotCount++;
			} else {
				return false;
			}
		}
		return atCount == 1 && dotCount >= 1;
	}
}


