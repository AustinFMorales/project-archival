#include "movie.h"

Movie::Movie(string m, int s, string d, string t, int y)
    : media(move(m)), stock(s), director(move(d)), title(move(t)), year(y) {}

Movie::Movie(const Movie &rhs) {
  media = rhs.media;
  stock = rhs.stock;
  director = rhs.director;
  title = rhs.title;
  year = rhs.year;
}

map<string, MovieFactory *> &Movie::getMap() {
  static map<string, MovieFactory *> factories;
  return factories;
}

void Movie::registerType(const string &type, MovieFactory *factory) {
  getMap().emplace(type, factory);
}

Movie *Movie::create(const string &type) {
  if (getMap().count(type) == 0) {
    return nullptr;
  }
  return getMap().at(type)->create();
}

string Movie::getTitle() const { return title; }

string Movie::getDirector() const { return director; }

int Movie::getYear() const { return year; }

int Movie::getStock() const { return stock; }

string Movie::getMedia() const { return media; }

void Movie::setTitle(string t) { title = move(t); }

void Movie::setDirector(string d) { director = move(d); }

void Movie::setYear(int y) { year = y; }

void Movie::setStock(int s) { stock = s; }

void Movie::setMedia(string m) { media = move(m); }

ostream &operator<<(ostream &os, const Movie &rhs) { return rhs.display(os); }

istream &operator>>(istream &is, Movie &rhs) { return rhs.input(is); }
