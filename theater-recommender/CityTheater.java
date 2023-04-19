import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.*;
import java.net.MalformedURLException;
import java.net.UnknownHostException;
import com.google.gson.*;
// class for storing movie theater data
public class CityTheater {
    private String name = "null";
    private String address = "null";
    private String zipcode = "null";
    private String phone = "null";
    private double rating = 0.0;

    // Constructors
    CityTheater(String n, String a, String z, String p, double r) {
        name = n;
        address = a;
        zipcode = z;
        phone = p;
        rating = r;
    }

    // getters 
    public String getName() {return name;}
    public String getAddress() {return address;}
    public String getZipcode() {return zipcode;}
    public String getPhone() {return phone;}
    public double getRating() {return rating;}
    // setters
    public void setName(String n) {name = n;}
    public void setAddress(String a) {address = a;}
    public void setZipcode(String z) {zipcode = z;}
    public void setPhone(String p) {phone = p;}
    public void setRating(double r) {rating = r;}

    public void display() {
        System.out.println("Theater Name: " + getName());
        System.out.println("Theater Address: " + getAddress());
        System.out.println("Theater ZipCode: " + getZipcode());
        System.out.println("Theater Contact: " + getPhone());
        System.out.println("Theater Rating: " + getRating() + "/5.0");
    }

    // opted not to make a separate deserialize JSON
    // method here because we have smaller amount of variables
    // to change here, so the constructor won't look messy
    
}