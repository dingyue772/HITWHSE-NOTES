package main.staff;// 工程师

import main.visitor.Visitor;

import java.util.Random;

public class Engineer extends Staff {
	private int kpi;// 员工KPI

    public Engineer(String name) {
        super(name);
    }

    @Override
    public void accept(Visitor visitor) {
        visitor.visitEngineer(this);
    }
    public int getKPI(){
		kpi = new Random().nextInt(10);
		return kpi;
	}
    // 工程师一年的代码数量
    public int getCodeLines() {
        return new Random().nextInt(100000);
    }
}