package main.visitor;

import main.staff.Engineer;
import main.staff.Manager;
import main.staff.Researcher;

public interface Visitor {

    // 访问工程师类型
    void visitEngineer(Engineer engineer);

    // 访问经理类型
    void visitManager(Manager manager);

    //访问研究员类型
    void visitResearcher(Researcher researcher);
}