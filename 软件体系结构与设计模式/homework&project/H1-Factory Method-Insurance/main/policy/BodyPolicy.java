package main.policy;

import main.insurance.AutoInsurance;
import main.insurance.BodyInjur;

public class BodyPolicy implements PolicyProducer {
    public AutoInsurance getAutoObj() {
       return new BodyInjur();
    }
}
