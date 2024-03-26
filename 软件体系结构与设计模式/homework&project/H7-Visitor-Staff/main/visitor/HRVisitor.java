package main.visitor;
// ÿһ����Ա��KPI
// ��Ŀ�����Ŀ������������������ķ���ƪ����ר�������������Ŀ��ɸ���

import main.staff.Engineer;
import main.staff.Manager;
import main.staff.Researcher;

public class HRVisitor implements Visitor {
    @Override
    public void visitEngineer(Engineer engineer) {
        System.out.println("����ʦ: " + engineer.getName() + ", KPI: " + engineer.getKPI() +
                ", ��������: " + engineer.getCodeLines());
    }

    @Override
    public void visitManager(Manager manager) {
        System.out.println("����: " + manager.getName() + ", KPI: " + manager.getKPI() +
                ", �²�Ʒ����: " + manager.getProducts());
    }

    public void visitResearcher(Researcher researcher){
        System.out.println("�о�Ա: " + researcher.getName() + ", KPI: " + researcher.getKPI()
                + ", ��������: " + researcher.getPapers() + ", ��Ŀ��ɸ����� " + researcher.getProjects() + ", ר����������� " + researcher.getPatents());

    }
}