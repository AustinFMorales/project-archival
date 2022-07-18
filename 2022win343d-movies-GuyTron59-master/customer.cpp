#include "customer.h"
#include "command.h"

Customer::Customer(int id, string last, string first)
    : id(id), lastName(move(last)), firstName(move(first)) {}

Customer::~Customer() {
  for (Movie *m : borrowed) {
    delete m;
  }

  for (Command *c : history) {
    delete c;
  }

  borrowed.clear();

  history.clear();
}

int Customer::getID() const { return id; }

string Customer::getLastName() const { return lastName; }

string Customer::getFirstName() const { return firstName; }

vector<Movie *> &Customer::getBorrowed() { return borrowed; }

vector<Command *> &Customer::getHistory() { return history; }

void Customer::addToHistory(Command *c) { history.insert(history.begin(), c); }

void Customer::setID(int i) { id = i; }

void Customer::setLastName(const string &last) { lastName = last; }

void Customer::setFirstName(const string &first) { firstName = first; }

void Customer::borrowMovie(Movie *&m) {
  m->setStock(m->getStock() - 1);
  borrowed.push_back(m);
  sort(borrowed.begin(), borrowed.end());
}

bool Customer::returnMovie(Movie *&m) {
  for (int i = 0; i < borrowed.size(); i++) {
    if (*m == *borrowed[i]) {
      m->setStock(m->getStock() + 1);
      borrowed.erase(borrowed.begin() + i);
      return true;
    }
  }

  return false;
}

void Customer::printHistory() const {
  cout << id << " " << lastName << " " << firstName << endl
       << "History:" << endl;
  for (const Command *c : history) {
    if (c != nullptr) {
      cout << *c << endl;
    }
  }
  cout << endl;
}

ostream &operator<<(ostream &os, const Customer &rhs) {
  os << rhs.id << " " << rhs.lastName << " " << rhs.firstName;
  return os;
}

istream &operator>>(istream &is, Customer &rhs) {
  is >> rhs.id >> rhs.lastName >> rhs.firstName;
  return is;
}