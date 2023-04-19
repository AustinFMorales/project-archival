// Austin Ferdinand Morales
// Professor Dimpsey
// CSS 436 - Program 2
// REST
// Reuse some of the libraries from program 1
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.*;
import java.net.MalformedURLException;
import java.net.UnknownHostException;
import com.google.gson.*;


public class MyCity {
    // first class - MyCity
    // utilizes the OpenWeatherAPI
    // to exclude fields in deserialization,
    // use transients as Gson ignores fields that are declared transient
    // https://www.baeldung.com/gson-exclude-fields-serialization
    // refer to the OpenWeatherAPI for json order
    // or to make your life easier, get json value
    // this is less efficient, but for brevity's sake
    // we need to extract certain values
    // https://stackoverflow.com/questions/20152710/gson-get-json-value-from-string
    private String city_name = "null";
    private String country_short = "null";
    private String weather_type = "null";
    private String weather_description = "null";
    // these are in kelvins!
    private double current_temp = 0.0;
    private double temp_min = 0.0;
    private double temp_max = 0.0;

    MyCity(String userJSON) {
        deserializeJSON(userJSON);
    }

    // setters
    public void setCityName(String cn) {
        city_name = cn;
    }

    public void setCountryShort(String cs) {
        country_short = cs;
    }

    public void setWeatherType(String wt) {
        weather_type = wt;
    }

    public void setWeatherDescription(String wd) {
        weather_description = wd;
    }

    public void setCurrentTemp(double ct) {
        current_temp = ct;
    }

    public void setTempMin(double t_min) {
        temp_min = t_min;
    }

    public void setTempMax(double t_max) {
        temp_max = t_max;
    }


    // getters
    public String getWeatherType() {
        return weather_type;
    }
    public String getWeatherDescription() {
        return weather_description;
    }

    public String getCityName() {
        return city_name;
    }

    public String getCountryShort() {
        return country_short;
    }

    public double getCurrentTemp() {
        return current_temp;
    }

    public double getTempMin() {
        return temp_min;
    }

    public double getTempMax() {
        return temp_max;
    }

    // from the userJSON
    // use GSON functions to extract data
    // https://stackoverflow.com/questions/48172941/getting-the-first-instance-in-a-json-array-java
    // THIS ONLY WORKS IF THE CONNECTION IS SUCCESS
    private void deserializeJSON(String UserJSON) {        
        // create it as a Json Object
        JsonObject JUser = new Gson().fromJson(UserJSON, JsonObject.class);
        // one thing to note about JSONs
        // [] - indicate arrays
        // {} - indicate objects
        // for cities
        String city = JUser.get("name").getAsString(); // built in getter in GSON for respective type
        setCityName(city);
        // to access object fields, we need to parse it
        // https://stackoverflow.com/questions/8233542/parse-a-nested-json-using-gson
        JsonParser parser = new JsonParser();
        // this is ugly looking, but this is the only way to easily 
        // access nested fields
        // AKA an object within an object
        JsonObject countryObject = parser.parse(UserJSON).getAsJsonObject().get("sys").getAsJsonObject();
        String country = countryObject.get("country").getAsString();
        setCountryShort(country);
        // weather we need to get it as a JSONArray, then extract values
        // https://stackoverflow.com/questions/48172941/getting-the-first-instance-in-a-json-array-java
        // https://stackoverflow.com/questions/41354932/getting-a-value-from-a-jsonarray-using-gson
        JsonArray weather_array = JUser.getAsJsonArray("weather");
        // get as a json element, then convert to an object
        // annoying part dealing with array elements
        JsonObject weather_array_object = weather_array.get(0).getAsJsonObject();
        String wt = weather_array_object.get("main").getAsString();
        setWeatherType(wt);
        String wd = weather_array_object.get("description").getAsString();
        setWeatherDescription(wd);
        // setting temperatures
        JsonObject main_object = parser.parse(UserJSON).getAsJsonObject().get("main").getAsJsonObject();
        double ct = main_object.get("temp").getAsDouble();
        setCurrentTemp(ct);
        double t_min = main_object.get("temp_min").getAsDouble();
        setTempMin(t_min);
        double t_max = main_object.get("temp_max").getAsDouble();
        setTempMax(t_max);
    }

    public void display() {
        // testing purposes
        System.out.println("You are in " + city_name);
        System.out.println("Country: " + country_short);
        System.out.println("With the weather being " + weather_type);
        System.out.println("Weather type: " + weather_description);
        System.out.println("It is " + KelvinsToFahrenheit(current_temp) +  "F today");
        System.out.println("Minimum temperature today: " + KelvinsToFahrenheit(temp_min) + "F");
        System.out.println("Maximum temperature today: " + KelvinsToFahrenheit(temp_max) + "F");

        if (KelvinsToFahrenheit(current_temp) >= 50) {
            System.out.println("You don't need a jacket today, it's warm out!");
        } else {
            System.out.println("You need a jacket, it's getting cold.");
        }

    }
    private int KelvinsToFahrenheit(double d) {
        int res = (int)((((d - 273.15) * 9.0) / 5.0) + 32.0);
        return res;
    }



    public static void main(String[] args) throws IOException {
        if (args.length < 1 || args.length > 1) {
            System.out.println("Invalid input, run with java MyCity <city name>");
            System.exit(1);
        }
        String cityName = args[0];

        try {
            System.out.println("-----------------------");
            System.out.println("Making API call");
            String OpenWeatherAPICallString = "https://api.openweathermap.org/data/2.5/weather?q=";
            OpenWeatherAPICallString += cityName + "&appid=";
            // we can hardcode the API keys in the program
            // not exactly the safest, but we're just experimenting with REST API
            String OpenWeatherAPI_Key = "dbc851365e0cce82e1963e092e3cac71";
            OpenWeatherAPICallString += OpenWeatherAPI_Key;
            URL url = new URL(OpenWeatherAPICallString);
            HttpURLConnection connection = (HttpURLConnection)url.openConnection();
            connection.setRequestMethod("GET");
            int statusCode = connection.getResponseCode();
            // System.out.println("Connected: " + OpenWeatherAPICallString);
            // System.out.println("Status Code: " + statusCode);
            // implement re-try logic, can we do this with a method instead?
            // deserialization must be done through a third party library
            BufferedReader reader = null;
            String json_string = "";
            if (isValidURL(connection, OpenWeatherAPICallString)) {
                reader = new BufferedReader(new InputStreamReader(connection.getInputStream()));
                json_string = JSONToString(reader);
                MyCity test = new MyCity(json_string);
                test.display();
            } else {
                System.out.println("failed finding weather data for " + cityName);
            }
            // test next API - open movie database api
            System.out.println("-----------------------");
            String OMDBCall = "http://www.omdbapi.com/?i=tt3896198&apikey=10f52b85&t=";
            OMDBCall += cityName;
            url = new URL(OMDBCall);
            connection = (HttpURLConnection)url.openConnection();
            connection.setRequestMethod("GET");
            // System.out.println("Connected: " + OMDBCall);
            // System.out.println("Status Code: " + statusCode);
            System.out.println("Wanna watch a film titled after your city?");
            System.out.println("Here's a film for you:"); 
            if (isValidURL(connection, OMDBCall)) {
                reader = new BufferedReader(new InputStreamReader(connection.getInputStream()));
                json_string = JSONToString(reader);
                CityMovie test2 = new CityMovie(json_string);
                test2.display();
            }  else {
                System.out.println("failed finding a movie that has " + cityName  + " in it");
            }
            System.out.println("-----------------------");
            // next test yelp API
            // for the yelp API's authenticators
            // we need to set request headers
            // to do this interactively and apply this concept,
            // play around with postman to see how it works
            String YelpCall = "https://api.yelp.com/v3/businesses/search?location=";
            // just in case a user adds in a city with spaces
            // yelp replaces spaces with %20
            cityName = cityName.replace(" ", "%20");
            YelpCall += cityName;
            YelpCall += "&term=movie&sort_by=best_match&limit=3";
            // the url called is searching movie theaters in seattle
            // so the user can see if there are any available movie theaters nearby
            // to watch the movie recommended to them
            url = new URL(YelpCall);
            connection = (HttpURLConnection)url.openConnection();
            // set the request headers
            connection.setRequestMethod("GET");
            String yelpOAuth = "Bearer 4CK7fLmL0DkegFt-s7e6IIV91gFQq43MhiG3gJOyYpwCuIznoDHlWJt0Qy0uQkIZKkheQvnd29YEsAMd-hHmhAXY-g4CSmceVAHUJ6qX8QsAPv_IlYDQSLiwEwneY3Yx";
            connection.setRequestProperty("Authorization", yelpOAuth);
            // System.out.println("Connected: " + YelpCall);
            // System.out.println("Status Code: " + statusCode);
            System.out.println("Visit these theatres near you to watch this film");
            System.out.println("-----------------------");
            // System.out.println(json_string);
            if (isValidURL(connection, YelpCall)) {
                try { 
                    // this is redundant, but sometimes there are IOException issues for inputstream if it can't find 
                    // movie theaters near the provided city location
                    // connection is successful, but the input stream is actually an error
                    // one of the limitations with HttpURLConnection compared to third party libraries such as HttpClient (Apache)
                    reader = new BufferedReader(new InputStreamReader(connection.getInputStream()));
                    json_string = JSONToString(reader);
                    List<CityTheater> nearby_theaters = buildTheaterList(json_string);
                    for (CityTheater current : nearby_theaters) {
                        current.display();
                        System.out.println();
                    }
                } catch (IOException e) {
                    System.out.println("failed finding nearby theaters in " + cityName);
                }
            } else {
                System.out.println("failed finding nearby theaters in " + cityName);
            }
        } catch (Exception E) {
            System.out.println("couldn't find city data for " + cityName);
            System.exit(1);
        }
    }

    // build a list of CityTheater objects 
    // by having a separate method to deserialize the JSON passed on by yelp
    public static List<CityTheater> buildTheaterList(String userJSON) throws IOException {
        List<CityTheater> res = new ArrayList<>();
        // JsonParser parser = new JsonParser();
        JsonObject theater_json = new Gson().fromJson(userJSON, JsonObject.class);
        JsonArray theater_array = theater_json.getAsJsonArray("businesses");
        for (JsonElement theater_element : theater_array) {
            JsonObject current = theater_element.getAsJsonObject();

            // extract the data 
            String name = current.get("name").getAsString();
            // access nested fields - address and zipcode
            // apparently you don't need to use parsers
            // here
            JsonObject location_obj = current.get("location").getAsJsonObject();
            String address = location_obj.get("address1").getAsString();
            String zipcode = location_obj.get("zip_code").getAsString();
            String phonenumber = current.get("display_phone").getAsString();
            double rating = current.get("rating").getAsDouble();
            CityTheater new_theater = new CityTheater(name, address, zipcode, phonenumber, rating);
            res.add(new_theater);
        }
        return res;
    }


    // simple method to convert JSON elements into string - won't have to redo work
    public static String JSONToString(BufferedReader c_reader) throws IOException {        
        String res = "";
        String line;
        while ((line = c_reader.readLine()) != null) {
            res += line;
        }
        return res;
    }

    
    // reuse this from Program1 to make sure this is a valid URL, but modify it in a sense
    // that it allows us to implement re-try logic
    // this seems somewhat redundant because we are making new URLs 
    // but java is pass by reference, so we can't deal anything about it
    public static boolean isValidURL(HttpURLConnection testCon, String user_url) throws MalformedURLException, UnknownHostException, InterruptedException {
        try {
            URL test_url = new URL(user_url);
            testCon = (HttpURLConnection)test_url.openConnection();
            int response = testCon.getResponseCode();
            // based on Google IoT and Amazon AWS definition of exponential backoff
            if (response >= 500) { // server errors, do exponential backoff
                int maximum_backoff = 10;
                boolean retry_status = true;
                int retries = 1;
                do {
                    // wait to make another connection again
                    // in milliseconds
                    Thread.sleep(retries * 100);
                    // then connect again
                    testCon = (HttpURLConnection)test_url.openConnection();
                    response = testCon.getResponseCode();
                    if (response >= 200 && response < 400) {
                        // successful call or redirects
                        // break out of the loop
                        retry_status = false;
                    } else if (response >= 400 && response < 500) {
                        // issue with client, not the server
                        return false;
                    } else if (response >= 500) {
                        retry_status = true;
                    }
                    retries++;
                } while (retries <= maximum_backoff && retry_status);
            }
        }   catch (MalformedURLException e) { // bad URL input exception
            return false;
        }   catch (UnknownHostException e) { // some domains are inaccessible
            return false;
        }   catch (IOException e) {
            return false;
        }
        return true;
    }
}
