#include "classic_movie.h"

ClassicMovieFactory::ClassicMovieFactory() { Movie::registerType("C", this); }

Movie *ClassicMovieFactory::create() const { return new ClassicMovie(); }

ClassicMovie::ClassicMovie(string media, int stock, string director,
                           string title, string actorFirstName,
                           string actorLastName, int month, int year)
    : Movie(move(media), stock, move(title), move(director), year),
      actorFirstName(move(actorFirstName)), actorLastName(move(actorLastName)),
      month(month) {}

ClassicMovie::ClassicMovie(const ClassicMovie &rhs)
    : ClassicMovie(rhs.media, rhs.stock, rhs.director, rhs.title,
                   rhs.actorFirstName, rhs.actorLastName, rhs.month, rhs.year) {
}

// setters
void ClassicMovie::setActorFirstName(string firstName) {
  actorFirstName = move(firstName);
}

void ClassicMovie::setActorLastName(string lastName) {
  actorLastName = move(lastName);
}

void ClassicMovie::setMonth(int m) { month = m; }

ostream &ClassicMovie::display(ostream &os) const {
  os << getStock() << ", " << getDirector() << ", " << getTitle() << ", "
     << actorFirstName << " " << actorLastName << ", " << month << " " << year
     << ", Media Type: " << getMedia();
  return os;
}

istream &ClassicMovie::input(istream &is) {
  is >> stock;
  is.get();
  is.get();
  getline(is, director, ',');
  is >> ws;
  getline(is, title, ',');
  is >> ws >> actorFirstName >> actorLastName >> month >> year;
  return is;
}

// getters
string ClassicMovie::getActorLastName() const { return actorLastName; }
string ClassicMovie::getActorFirstName() const { return actorFirstName; }
int ClassicMovie::getMonth() const { return month; }

bool ClassicMovie::operator<(const Movie &rhs) const {
  auto rhsCopy = dynamic_cast<const ClassicMovie &>(rhs);
  return (year < rhsCopy.year ||
          (year == rhsCopy.year &&
           (month < rhsCopy.month ||
            (month == rhsCopy.month &&
             (actorFirstName + actorLastName) <
                 (rhsCopy.actorFirstName + rhsCopy.actorLastName)))));
}

bool ClassicMovie::operator>(const Movie &rhs) const {
  auto rhsCopy = dynamic_cast<const ClassicMovie &>(rhs);
  return (year > rhsCopy.year ||
          (year == rhsCopy.year &&
           (month > rhsCopy.month ||
            (month == rhsCopy.month &&
             (actorFirstName + actorLastName) >
                 (rhsCopy.actorFirstName + rhsCopy.actorLastName)))));
}

bool ClassicMovie::operator==(const Movie &rhs) const {
  auto rhsCopy = dynamic_cast<const ClassicMovie &>(rhs);
  return media == rhsCopy.media && year == rhsCopy.year &&
         month == rhsCopy.month &&
         (actorFirstName + actorLastName) ==
             (rhsCopy.actorFirstName + rhsCopy.actorLastName);
  ;
}

bool ClassicMovie::operator!=(const Movie &rhs) const {
  auto rhsCopy = dynamic_cast<const ClassicMovie &>(rhs);
  return !(*this == rhsCopy);
}

void ClassicMovie::setUsingSortingAttributes(const string &sortingAttributes) {
  stringstream ss(sortingAttributes);
  ss >> month >> year >> actorFirstName >> actorLastName;
}

ClassicMovieFactory anonymousClassicMovieFactory;