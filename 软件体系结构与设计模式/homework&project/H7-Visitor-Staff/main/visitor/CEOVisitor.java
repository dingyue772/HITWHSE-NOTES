package main.visitor;// CEO访问者

//KPI：Key Performance Indication
// CEO关心经理们的KPI与新产品数量；关心工程师的KPI

import main.staff.Engineer;
import main.staff.Manager;
import main.staff.Researcher;

public class CEOVisitor implements Visitor {
    @Override
    public void visitEngineer(Engineer engineer) {
        System.out.println("工程师: " + engineer.getName() + ", KPI: " + engineer.getKPI());
    }

    @Override
    public void visitManager(Manager manager) {
        System.out.println("经理: " + manager.getName() + ", KPI: " + manager.getKPI() +
                ", 新产品数量: " + manager.getProducts());
    }

    public void visitResearcher(Researcher researcher){
		System.out.println("研究员: " + researcher.getName() + ", KPI: " + researcher.getKPI());

	}
}