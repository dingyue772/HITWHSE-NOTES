package main.visitor;

import main.staff.Engineer;
import main.staff.Manager;
import main.staff.Researcher;

public interface Visitor {

    // ���ʹ���ʦ����
    void visitEngineer(Engineer engineer);

    // ���ʾ�������
    void visitManager(Manager manager);

    //�����о�Ա����
    void visitResearcher(Researcher researcher);
}