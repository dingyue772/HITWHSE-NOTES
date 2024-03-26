package main.factory;


import main.condo.Condo;
import main.house.House;
import main.condo.SupCondo;
import main.house.SupHouse;
import main.semide.SemiDetacher;
import main.semide.SupSemiDe;

public class SuperFactory extends BuildingFactory {

   public House getHouse(){
      return new SupHouse();
   }

   public Condo getCondo(){
	  return new SupCondo();
   }

   @Override
   public SemiDetacher getSemiDetacher() {
      return new SupSemiDe();
   }
}
