#ifndef CUSTOMER_H
#define CUSTOMER_H

#include "movie.h"
#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

class Command;

class Customer {
protected:
  // data members for our customer
  int id{0};
  string lastName;
  string firstName;
  vector<Command *> history;
  vector<Movie *> borrowed;

public:
  // Default constructor and constructors
  Customer() = default;
  Customer(int id, string last, string first);
  // Destrucor to clean history and borrowed
  ~Customer();
  // These operators are not allowed
  Customer(const Customer &rhs) = delete;
  Customer(Customer &&other) = delete;
  Customer &operator=(const Customer &other) = delete;
  Customer &operator=(const Customer &&other) = delete;
  int getID() const;
  string getFirstName() const;
  string getLastName() const;
  vector<Movie *> &getBorrowed();
  vector<Command *> &getHistory();
  void addToHistory(Command *c);
  void setID(int i);
  void setFirstName(const string &first);
  void setLastName(const string &last);
  void borrowMovie(Movie *&m); // or Movie *
  bool returnMovie(Movie *&m);
  void printHistory() const;
  friend ostream &operator<<(ostream &os, const Customer &rhs);
  friend istream &operator>>(istream &is, Customer &rhs);
};

#endif