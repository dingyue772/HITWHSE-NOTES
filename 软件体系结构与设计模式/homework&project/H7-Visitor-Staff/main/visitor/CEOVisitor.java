package main.visitor;// CEO������

//KPI��Key Performance Indication
// CEO���ľ����ǵ�KPI���²�Ʒ���������Ĺ���ʦ��KPI

import main.staff.Engineer;
import main.staff.Manager;
import main.staff.Researcher;

public class CEOVisitor implements Visitor {
    @Override
    public void visitEngineer(Engineer engineer) {
        System.out.println("����ʦ: " + engineer.getName() + ", KPI: " + engineer.getKPI());
    }

    @Override
    public void visitManager(Manager manager) {
        System.out.println("����: " + manager.getName() + ", KPI: " + manager.getKPI() +
                ", �²�Ʒ����: " + manager.getProducts());
    }

    public void visitResearcher(Researcher researcher){
		System.out.println("�о�Ա: " + researcher.getName() + ", KPI: " + researcher.getKPI());

	}
}