package main.policy;

import main.insurance.AutoInsurance;
import main.insurance.ComCover;

public class ComPolicy implements PolicyProducer {

    public AutoInsurance getAutoObj() {
        return new ComCover();
    }
}
