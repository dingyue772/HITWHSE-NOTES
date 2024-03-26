package main.visitor;
// CTO关心代码行数与产品数量

import main.staff.Engineer;
import main.staff.Manager;
import main.staff.Researcher;

public class CTOVisitor implements Visitor {
    @Override
    public void visitEngineer(Engineer engineer) {
        System.out.println("工程师: " + engineer.getName() + ", 代码行数: " + engineer.getCodeLines());
    }

    @Override
    public void visitManager(Manager manager) {
        System.out.println("经理: " + manager.getName() + ", 产品数量: " + manager.getProducts());
    }

    public void visitResearcher(Researcher researcher){
		System.out.println("研究员: " + researcher.getName() + ", 论文数量: " + researcher.getPapers());

	}
}