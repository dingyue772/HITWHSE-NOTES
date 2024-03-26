package main;// Ա��ҵ�񱨱���
// CompositeStructure��
import main.staff.Staff;
import main.visitor.Visitor;

import java.util.*;

public class BusinessReport {

    private ArrayList<Staff> staff = new ArrayList<>();

    public void attach(Staff s){
		staff.add(s);
	}

    /**
     * Ϊ������չʾ�������е�Staff����accept������
     * @param visitor ��˾�߲㣬��CEO��CTO
     */
    public void acceptAll(Visitor visitor) {
        for (int k=0; k< staff.size(); k++) {
		    staff.get(k).accept(visitor);
        }
    }
}