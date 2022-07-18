#ifndef STORE_H
#define STORE_H

#include "classic_movie.h"
#include "comedy_movie.h"
#include "command.h"
#include "customer.h"
#include "drama_movie.h"
#include "hash_table.h"
#include "movie.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <sstream>
#include <vector>

using namespace std;

class Store {
private:
  // Container of *Customers
  // each customer has a unique id
  // would be great to have an operator overload
  // for comparing ID
  HashTable<Customer *> customers;
  // Container of *Movies
  // two options of containing
  // three vector types
  // OR STL Map
  // easily sort them using std::sort and a comparator
  /*
  vector<ComedyMovie *> comedies;
  vector<DramaMovie *> dramas;
  vector<ClassicMovie *> classics;
  */
  // you can do stuff like this
  // movieDatabase["D"].push_back(myMovie);
  // initialize the vector based on genres first
  map<string, vector<Movie *>> movies;
  queue<Command *> commands;
  // Container of *Commands - Implement later
  // make this a queue to process commands
  // similar to jollybanker
public:
  // Destroy all allocated memory
  Store();
  ~Store(); // = default; temporary? look at store.cpp
  // The following operators are not allowed
  Store(const Store &rhs) = delete;
  Store(const Store &&rhs) = delete;
  Store &operator=(const Store &rhs) = delete;
  Store &operator=(const Store &&rhs) = delete;

  // process these commands in order
  bool readMovies(const string &file);
  bool readCustomers(const string &file);
  bool readCommands(const string &file);
  void executeCommands();
  Customer *createCustomer(int id, const string &lastName,
                           const string &firstName);
  // Because of how our factories are set
  // we will have to rely on using setters or
  // operator>> overload, so use the type
  // as a parameter to create a movie
  Movie *createMovie(const string &type);
  // add a customer to hashtable
  // if they already exist, return false
  // otherwise, return true
  bool addCustomer(Customer *rhs);
  // Add movie to the right container
  // and sort on the end
  // return true if successful
  // return false if EXACT duplicates exist
  bool addMovie(Movie *rhs);
  // Print the movie inventory
  // Comedy first, drama second,
  // and drama last.
  void printInventory();

  // Borrow and return methods
  // base this off data4commands.txt
  // there is redundancy here, figure out a way to remove this
  // hint: command class and use our operator overloads
  // make temp objects or dereference pointers to compare objects
  bool borrowComedy(string title, int year);
  bool borrowDrama(string director, string title);
  bool borrowClassic(int month, int year, string actorFirst, string actorLast);
  bool returnComedy(string title, int year);
  bool returnDrama(string director, string title);
  bool returnClassic(int month, int year, string actorFirst, string actorLast);

  Customer *getCustomer(const int &id);
  // Movie *getMovie()
};

#endif