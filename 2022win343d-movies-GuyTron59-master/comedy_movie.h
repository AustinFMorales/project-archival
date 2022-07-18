#ifndef COMEDY_MOVIE_H
#define COMEDY_MOVIE_H

#include "movie.h"

using namespace std;

class ComedyMovie;

class ComedyMovieFactory : public MovieFactory {
public:
  ComedyMovieFactory();
  Movie *create() const override;
};

class ComedyMovie : public Movie {
private:
  // output and input operator overloads, helpful for
  // parsing in movies
  ostream &display(
      ostream &os) const override;
  istream &input(istream &is) override;

public:
  // base on attributes 
  bool operator<(const Movie &rhs) const override;
  bool operator>(const Movie &rhs) const override;
  bool operator==(const Movie &rhs) const override;
  bool operator!=(const Movie &rhs) const override;
  // setters

  void setUsingSortingAttributes(const string &sortingAttributes) override;
};

#endif