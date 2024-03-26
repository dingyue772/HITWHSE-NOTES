package main.policy;

import main.insurance.AutoInsurance;
import main.insurance.Collision;

public class CollPolicy implements PolicyProducer {
    public AutoInsurance getAutoObj() {
        return new Collision();
    }
}
