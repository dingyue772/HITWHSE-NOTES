package main.insurance;

public class LuxuryInsur implements AutoInsurance{
    private String description;

    public String getInfo(){
        description = "Luxury Car Insurance:\n\nDesigned for high-end vehicles, "+
        "it includes comprehensive and collision coverage, liability protection, "+
        "and may offer extras like roadside assistance.";
        return description;
    }
}
