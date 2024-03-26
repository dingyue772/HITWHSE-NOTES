package main.state; /*=============================================================*/
 /* This is the main.state.State class in the main.state.State pattern. This class has 3 subclasses:
 /* main.state.NoTransactionFeeState,main.state.TransactionFeeState and main.state.OverDrawnState.
 /*=============================================================*/

import main.BankContext;

public abstract class State {
    private String acctNumber;
    protected double balance =0;
    protected String state;
    protected static final String NOFEESTATE="NoTransFeeState";
    protected static final String FEESTATE="TransFeeState";
    protected static final String OVERDRAWNSTATE="main.state.OverDrawnState";
    protected static final String ERRORSTATE="ErrorState";
    protected static final String TAXSTATE="main.state.TaxState";

    protected State stateObj;
    protected BankContext context;

    public void changeState(){
		balance = context.getBalance();
	   if(balance<0 && balance >= BankContext.OVERDRAW_LIMIT) { // -1000到0为OverDrawnState
		   state = OVERDRAWNSTATE;
		   System.out.println("Now it's main.state.OverDrawnState!");
	   }
	   else if (balance >= BankContext.TAX_BALANCE) { // 100000往上为TaxState
		   state = TAXSTATE;
		   System.out.println("Now it's main.state.TaxState!");
	   }
	   else if (balance >= BankContext.MIN_BALANCE){ // 2000往上为NoTransactionFeeState
		   state = NOFEESTATE;
		   System.out.println("Now it's NoTransFeeState!");
	   }
	   else if (balance >= 0 && balance < BankContext.MIN_BALANCE){ // 0到2000为TransactionFeeState
		   state = FEESTATE;
		   System.out.println("Now it's TransFeeState!");
	   }

	   else
	      state = ERRORSTATE;
	   passStateObjToContext();
	}
	public void passStateObjToContext(){
	   if(state.equals(OVERDRAWNSTATE)  )
	      stateObj = new OverDrawnState();
	   else if (state.equals(NOFEESTATE) )
	      stateObj = new  NoTransactionFeeState();
	   else if (state.equals(FEESTATE))
	      stateObj = new  TransactionFeeState();
	   else if (state.equals(TAXSTATE))
	   	  stateObj = new TaxState();
	   context.setStateObj(stateObj);
	}
	public String getAcctNum(){
	   return acctNumber;
	}
	public String getState(){
	   return state;
	}
	public void setContext(BankContext context){
	    this.context = context;
    }

    public abstract void deposit(double amount);
    public abstract void withdraw(double amount);
    public abstract boolean isOverDrawnLimitReached();
}
