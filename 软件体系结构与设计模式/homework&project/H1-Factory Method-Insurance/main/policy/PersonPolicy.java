package main.policy;

import main.insurance.AutoInsurance;
import main.insurance.PersonInjur;

public class PersonPolicy implements PolicyProducer {

   public AutoInsurance getAutoObj() {
      return new PersonInjur();
   }
}
