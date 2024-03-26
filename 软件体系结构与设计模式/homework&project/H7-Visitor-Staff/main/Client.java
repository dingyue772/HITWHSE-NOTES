package main;

import main.staff.Engineer;
import main.staff.Manager;
import main.staff.Researcher;
import main.visitor.CEOVisitor;
import main.visitor.CTOVisitor;
import main.visitor.HRVisitor;

public class Client {

    public static void main(String[] args) {

        BusinessReport report = new BusinessReport();

         report.attach(new Manager("main.staff.Manager-A"));
		 report.attach(new Engineer("main.staff.Engineer-A"));
		 report.attach(new Engineer("main.staff.Engineer-B"));
		 report.attach(new Engineer("main.staff.Engineer-C"));
		 report.attach(new Manager("main.staff.Manager-B"));
         report.attach(new Engineer("main.staff.Engineer-D"));
         report.attach(new Researcher("Anna-Sun"));
         report.attach(new Researcher("Andy-Sun"));



        // ��������
        System.out.println("=========== CEO������ ===========");
        report.acceptAll(new CEOVisitor());
        System.out.println("=========== CTO������ ===========");
        report.acceptAll(new CTOVisitor());

        System.out.println("=========== HR ������ ===========");
		report.acceptAll(new HRVisitor());
    }
}