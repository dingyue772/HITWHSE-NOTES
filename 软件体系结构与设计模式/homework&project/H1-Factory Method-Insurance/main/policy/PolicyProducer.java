package main.policy;

import main.insurance.AutoInsurance;

public interface PolicyProducer
{
    public AutoInsurance getAutoObj();
}
