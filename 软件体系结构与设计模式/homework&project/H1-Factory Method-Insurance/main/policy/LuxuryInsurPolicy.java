package main.policy;

import main.insurance.AutoInsurance;
import main.insurance.LuxuryInsur;

public class LuxuryInsurPolicy implements PolicyProducer {
    public AutoInsurance getAutoObj() {
        return new LuxuryInsur();
    }
}
