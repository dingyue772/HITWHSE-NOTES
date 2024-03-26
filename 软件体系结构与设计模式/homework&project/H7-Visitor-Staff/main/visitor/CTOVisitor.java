package main.visitor;
// CTO���Ĵ����������Ʒ����

import main.staff.Engineer;
import main.staff.Manager;
import main.staff.Researcher;

public class CTOVisitor implements Visitor {
    @Override
    public void visitEngineer(Engineer engineer) {
        System.out.println("����ʦ: " + engineer.getName() + ", ��������: " + engineer.getCodeLines());
    }

    @Override
    public void visitManager(Manager manager) {
        System.out.println("����: " + manager.getName() + ", ��Ʒ����: " + manager.getProducts());
    }

    public void visitResearcher(Researcher researcher){
		System.out.println("�о�Ա: " + researcher.getName() + ", ��������: " + researcher.getPapers());

	}
}