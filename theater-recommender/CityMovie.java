
// because we are not working in eclipse
// or intelliJ, this must be manually done
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.*;
import java.net.MalformedURLException;
import java.net.UnknownHostException;
import com.google.gson.*;


// class for holding Movie data
// using a city's name
// this class stores parts
// of JSON data from the
// Open Movie Database API
public class CityMovie {
    private String title = "null";
    private String release_date = "null";
    private String duration = "null";
    private String genre = "null";
    private String director = "null";
    private String plot = "null";
    private String imdb_rating = "null";

    // getters
    public String getTitle() {return title;}
    public String getReleaseDate() {return release_date;}
    public String getDuration() {return duration;}
    public String getGenre() {return genre;}
    public String getDirector() {return director;}
    public String getPlot() {return plot;}
    public String getIMDBRating() {return imdb_rating;}

    
    // constructs the CityMovie through json
    CityMovie(String userJSON) {
        deserializeJSON(userJSON);
    }
    
    // setters
    public void setTitle(String t) {title = t;}
    public void setReleaseDate(String rd) {release_date = rd;}
    public void setGenre(String g) {genre = g;}
    public void setDirector(String d) {director = d;}
    public void setPlot(String p) {plot = p;}
    public void setIMDBRating(String imdb_r) {imdb_rating = imdb_r;}
    
    // same idea as MyCity class
    // its easier to parse data through the
    // Open Movie Database API as the fields are simpler
    // no nested objects or array access to consider
    private void deserializeJSON(String userJSON) {
        JsonObject movie_json = new Gson().fromJson(userJSON, JsonObject.class);
        String title = movie_json.get("Title").getAsString();
        setTitle(title);
        String releasedate = movie_json.get("Released").getAsString();
        setReleaseDate(releasedate);
        String genre = movie_json.get("Genre").getAsString();
        setGenre(genre);
        String director = movie_json.get("Director").getAsString();
        setDirector(director);
        String plotline = movie_json.get("Plot").getAsString();
        setPlot(plotline);
        String rating = movie_json.get("imdbRating").getAsString();
        setIMDBRating(rating);
    }

    // test display 
    public void display() {
        System.out.println("Title: " + getTitle());
        System.out.println("Release Date: " + getReleaseDate());
        System.out.println("Genre: " + getGenre());
        System.out.println("Director: " + getDirector());
        System.out.println("Plot: " + getPlot());
        System.out.println("IMDB Rating: " + getIMDBRating());
    }
}