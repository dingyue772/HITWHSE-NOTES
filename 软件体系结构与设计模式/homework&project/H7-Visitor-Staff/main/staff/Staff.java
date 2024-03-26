package main.staff;// main.staff.Staff class

import main.visitor.Visitor;

import java.util.Random;

public abstract class Staff {

    private String name;
   // private int kpi;// Ա��KPI

    public Staff(String name) {
        this.name = name;
        //kpi = new Random().nextInt(10);
    }
    public String getName(){
		return name;
	}

    // ���ķ���������Visitor�ķ���
    public abstract void accept(Visitor visitor);
}