#include "drama_movie.h"

DramaMovieFactory::DramaMovieFactory() { Movie::registerType("D", this); }

Movie *DramaMovieFactory::create() const { return new DramaMovie(); }

ostream &DramaMovie::display(ostream &os) const {
  os << getStock() << ", " << getDirector() << ", " << getTitle() << ", "
     << year << ", Media Type: " << getMedia();
  return os;
}

istream &DramaMovie::input(istream &is) {
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

bool DramaMovie::operator<(const Movie &rhs) const {
  // Call dynamic cast
  auto rhsCopy = dynamic_cast<const DramaMovie &>(rhs);
  return (director < rhsCopy.director ||
          (director == rhsCopy.director && title < rhsCopy.title));
}

bool DramaMovie::operator>(const Movie &rhs) const {
  auto rhsCopy = dynamic_cast<const DramaMovie &>(rhs);
  return (director > rhsCopy.director ||
          (director == rhsCopy.director && title > rhsCopy.title));
}

bool DramaMovie::operator==(const Movie &rhs) const {
  // for DramaMovie, director and title are enough (and all that should be
  // used) to determine if two movies are equal; for ComedyMovie, title and
  // year are enough; and for ClassicMovie, release date (month and year) and
  // major actor are enough
  auto rhsCopy = dynamic_cast<const DramaMovie &>(rhs);
  return media == rhsCopy.media && director == rhsCopy.director &&
         title == rhsCopy.title;
}

bool DramaMovie::operator!=(const Movie &rhs) const { return !(*this == rhs); }

void DramaMovie::setUsingSortingAttributes(const string &sortingAttributes) {
  stringstream ss(sortingAttributes);
  getline(ss, director, ',');
  ss.get();
  getline(ss, title, ',');
}

DramaMovieFactory anonymousDramaMovieFactory;