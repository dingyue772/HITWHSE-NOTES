package main.visitor;
// 每一类人员的KPI
// 项目完成数目，代码完成行数，论文发表篇数，专利发表个数，项目完成个数

import main.staff.Engineer;
import main.staff.Manager;
import main.staff.Researcher;

public class HRVisitor implements Visitor {
    @Override
    public void visitEngineer(Engineer engineer) {
        System.out.println("工程师: " + engineer.getName() + ", KPI: " + engineer.getKPI() +
                ", 代码行数: " + engineer.getCodeLines());
    }

    @Override
    public void visitManager(Manager manager) {
        System.out.println("经理: " + manager.getName() + ", KPI: " + manager.getKPI() +
                ", 新产品数量: " + manager.getProducts());
    }

    public void visitResearcher(Researcher researcher){
        System.out.println("研究员: " + researcher.getName() + ", KPI: " + researcher.getKPI()
                + ", 论文数量: " + researcher.getPapers() + ", 项目完成个数： " + researcher.getProjects() + ", 专利发表个数： " + researcher.getPatents());

    }
}