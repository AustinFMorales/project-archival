#ifndef MOVIE_H
#define MOVIE_H

#include <iostream>
#include <map>
#include <sstream>
#include <string>

using namespace std;

// movies must keep track of customers borrowing and customer must keep track of
// movies borrowed

// forward declaration
class Movie;

// Abstract of Movie Factory, each subclass of movie needs to define
// its own factory
class MovieFactory {
public:
  virtual Movie *create() const = 0;
};

// undefined references mean that subclasses need to have their own explicit
// constructor
class Movie {
  // register types of movies to register factories
  static map<string, MovieFactory *> &getMap();
  // NVI idioms for printing to be used by inherited movies.
  virtual ostream &display(ostream &os) const = 0;
  virtual istream &input(istream &is) = 0;

protected:
  // protected data members to be inherited to other classes
  string media{"D"};
  int stock{0};
  string director;
  string title;
  int year{0};

public:
  // default destructor, the self-registering classes
  // through this super class using global factories
  // will not be detected
  virtual ~Movie() = default;
  Movie() = default;
  Movie(string m, int s, string d, string t, int y);
  // copy constructor allowed, but move and assignment operator
  Movie(const Movie &rhs);
  Movie(Movie &&other) = delete;
  Movie &operator=(const Movie &other) = delete;
  Movie &operator=(const Movie &&other) = delete;

  // register type of movie through factories
  static void registerType(const string &type, MovieFactory *factory);
  static Movie *create(const string &type);
  // getters and setters
  string getDirector() const;
  string getTitle() const;
  int getYear() const;
  int getStock() const;
  string getMedia() const;
  void setDirector(string d);
  void setTitle(string t);
  void setYear(int y);
  void setStock(int s);
  void setMedia(string m);

  // use this in tandem with command to actually build
  // movies to search in store.cpp
  virtual void setUsingSortingAttributes(const string &sortingAttributes) = 0;

  // operator overloads for sub classes of movies
  // to override and base on sorting attributes
  virtual bool operator<(const Movie &rhs) const = 0;
  virtual bool operator>(const Movie &rhs) const = 0;
  virtual bool operator==(const Movie &rhs) const = 0;
  virtual bool operator!=(const Movie &os) const = 0;
  friend ostream &operator<<(ostream &os, const Movie &rhs);
  friend istream &operator>>(istream &is, Movie &rhs);
};

#endif