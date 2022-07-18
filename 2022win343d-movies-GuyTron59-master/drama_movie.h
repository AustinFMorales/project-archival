#ifndef DRAMA_MOVIE_H
#define DRAMA_MOVIE_H

#include "movie.h"

using namespace std;

class DramaMovie;

class DramaMovieFactory : public MovieFactory {
public:
  DramaMovieFactory();
  Movie *create() const override;
};

class DramaMovie : public Movie {
private:
  // output and input operator overloads, helpful for
  // parsing in movies
  ostream &display(ostream &os) const override;
  istream &input(istream &is) override;

public:
  bool operator<(const Movie &rhs) const override;
  bool operator>(const Movie &rhs) const override;
  bool operator==(const Movie &rhs) const override;
  bool operator!=(const Movie &rhs) const override;

  void setUsingSortingAttributes(const string &sortingAttributes) override;
};

#endif
