package main.staff;// main.staff.Staff class

import main.visitor.Visitor;

import java.util.Random;

public abstract class Staff {

    private String name;
   // private int kpi;// 员工KPI

    public Staff(String name) {
        this.name = name;
        //kpi = new Random().nextInt(10);
    }
    public String getName(){
		return name;
	}

    // 核心方法，接受Visitor的访问
    public abstract void accept(Visitor visitor);
}