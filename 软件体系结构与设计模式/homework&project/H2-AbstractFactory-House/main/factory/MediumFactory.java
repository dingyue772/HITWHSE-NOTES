package main.factory;


import main.condo.Condo;
import main.house.House;
import main.condo.MedCondo;
import main.house.MedHouse;
import main.semide.MedSemiDe;
import main.semide.SemiDetacher;

public class MediumFactory extends BuildingFactory {

   public House getHouse(){
      return new MedHouse();
   }

   public Condo getCondo(){
	  return new MedCondo();
   }

   public SemiDetacher getSemiDetacher() {
      return new MedSemiDe();
   }
}
