package main;// 员工业务报表类
// CompositeStructure类
import main.staff.Staff;
import main.visitor.Visitor;

import java.util.*;

public class BusinessReport {

    private ArrayList<Staff> staff = new ArrayList<>();

    public void attach(Staff s){
		staff.add(s);
	}

    /**
     * 为访问者展示报表，所有的Staff对象都accept访问者
     * @param visitor 公司高层，如CEO、CTO
     */
    public void acceptAll(Visitor visitor) {
        for (int k=0; k< staff.size(); k++) {
		    staff.get(k).accept(visitor);
        }
    }
}