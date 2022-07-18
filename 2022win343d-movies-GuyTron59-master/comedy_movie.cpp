#include "comedy_movie.h"

ComedyMovieFactory::ComedyMovieFactory() { Movie::registerType("F", this); }

Movie *ComedyMovieFactory::create() const { return new ComedyMovie(); }

ostream &ComedyMovie::display(ostream &os) const {
  os << getStock() << ", " << getDirector() << ", " << getTitle() << ", "
     << year << ", Media Type: " << getMedia();
  return os;
}

istream &ComedyMovie::input(istream &is) {
  is >> stock;
  is.get();
  is.get();
  getline(is, director, ',');
  is >> ws;
  getline(is, title, ',');
  is >> ws;
  is >> year;
  return is;
}

bool ComedyMovie::operator<(const Movie &rhs) const {
  auto rhsCopy = dynamic_cast<const ComedyMovie &>(rhs);
  return title < rhsCopy.title ||
         (title == rhsCopy.title && year < rhsCopy.year);
}

bool ComedyMovie::operator>(const Movie &rhs) const {
  auto rhsCopy = dynamic_cast<const ComedyMovie &>(rhs);
  return title > rhsCopy.title ||
         (title == rhsCopy.title && year > rhsCopy.year);
}

bool ComedyMovie::operator==(const Movie &rhs) const {
  auto rhsCopy = dynamic_cast<const ComedyMovie &>(rhs);
  return media == rhsCopy.media && title == rhsCopy.title &&
         year == rhsCopy.year;
}

bool ComedyMovie::operator!=(const Movie &rhs) const { return !(*this == rhs); }

void ComedyMovie::setUsingSortingAttributes(const string &sortingAttributes) {
  stringstream ss(sortingAttributes);
  getline(ss, title, ',');
  ss.get();
  ss >> year;
}

ComedyMovieFactory anonymousComedyMovieFactory;