package main;

import main.sort.SortAlgorithm;


// Configured with a ConcreteStrategy object and maintains a reference to a Strategy object
public class Context {

    SortAlgorithm alg;
    private long startTime;
    private long endTime;

    // Constructor
    public Context(SortAlgorithm alg) {
        this.alg = alg;
    }

    public int[] sortIntArray(int[] a) {
        return this.alg.sort(a, this);
    }

    public void startExecution(){
		startTime = System.currentTimeMillis();
    }
    public void endExecution(){
		endTime = System.currentTimeMillis();
    }
    public long getExeTime(){
        System.out.println("��ʼʱ�䣺" + startTime);
        System.out.println("\n����ʱ�䣺" + endTime);
		long exeTime=0;
		exeTime = endTime-startTime;
		return exeTime;
    }
}