
#ifndef CLASSIC_MOVIE_H
#define CLASSIC_MOVIE_H

#include "movie.h"

using namespace std;

class ClassicMovie;

class ClassicMovieFactory : public MovieFactory {
public:
  ClassicMovieFactory();
  Movie *create() const override;
};

class ClassicMovie : public Movie {
private:
  // output and input operator overloads, helpful for
  // parsing in movies
  ostream &display(ostream &os) const override;
  istream &input(istream &is) override;

protected:
  string actorFirstName;
  string actorLastName;
  int month{0};

public:
  ClassicMovie() = default;
  ClassicMovie(string media, int stock, string director, string title,
               string actorFirstName, string actorLastName, int month,
               int year);

  ~ClassicMovie() override = default;
  ClassicMovie(ClassicMovie &&other) = delete;
  ClassicMovie &operator=(const ClassicMovie &other) = delete;
  ClassicMovie &operator=(const ClassicMovie &&other) = delete;

  ClassicMovie(const ClassicMovie &rhs);
  bool operator<(const Movie &rhs) const override;
  bool operator>(const Movie &rhs) const override;
  bool operator==(const Movie &rhs) const override;
  bool operator!=(const Movie &rhs) const override;
  // getters
  string getActorLastName() const;
  string getActorFirstName() const;
  int getMonth() const;
  // setters
  void setActorFirstName(string firstName);
  void setActorLastName(string lastName);
  void setMonth(int m);

  void setUsingSortingAttributes(const string &sortingAttributes) override;
};

#endif