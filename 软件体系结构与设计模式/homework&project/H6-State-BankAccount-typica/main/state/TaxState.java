package main.state;

import main.BankContext;

public class TaxState extends State{
    private boolean overDrawnLismitFlag = false;

    public TaxState(){
        state = TAXSTATE;
    }
    public void deposit(double amount){
        System.out.println("This is main.state.TaxState's deposit");
        if( amount>0 ){
            amount = amount*(1-0.05);
            balance = context.getBalance() + amount;
            context.updateBalance(balance);
            changeState();
        }
        else{
            System.out.println("Deposit amount cannot be 0 or negative");
        }
    }
    public void withdraw(double amount){
        if( amount>0 ){
            if ((context.getBalance() - BankContext.TRANS_FEE_NORMAL - amount) > BankContext.OVERDRAW_LIMIT) {
                balance = context.getBalance()  - BankContext.TRANS_FEE_NORMAL;
                balance = balance - amount;
                context.updateBalance(balance);
                changeState();
            }
            else{
                overDrawnLismitFlag = true;
            }
        }
        else{
            System.out.println("withdraw amount cannot be 0 or negative");
        }

    }
    public boolean isOverDrawnLimitReached(){
        return overDrawnLismitFlag;
    }
}
