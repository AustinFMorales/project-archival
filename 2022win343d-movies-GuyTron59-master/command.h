#ifndef COMMAND_H
#define COMMAND_H

#include "hash_table.h"
#include "movie.h"
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

// forward declaration
class Command;

// Abstract of Command Factory, each subclass of movie needs to define
// its own factory
class CommandFactory {
public:
  virtual Command *create() const = 0;
};

// use forward declaration
// to 
class Customer;

class Command {
private:
  // register types of commands to register factories
  static map<string, CommandFactory *> &getMap();
  // NVI idioms for printing to be used by inherited movies.
  virtual ostream &display(ostream &os) const = 0;
  virtual istream &input(istream &is) = 0;

protected:
  // should be inherited by derived classes
  bool executed = false;

public:
  virtual ~Command() = default;
  Command() = default;
  Command(const Command &rhs) = delete;
  Command(Command &&other) = delete;
  Command &operator=(const Command &other) = delete;
  Command &operator=(const Command &&other) = delete;

  // use self register for the command factories 
  // pisan allowed us to put the headers in the same file
  // due to how similar they are
  static void registerType(const string &type, CommandFactory *factory);
  static Command *create(const string &type);
  virtual bool execute(HashTable<Customer *> &customers,
                       map<string, vector<Movie *>> &movies) = 0;
  bool hasExecuted();
  friend ostream &operator<<(ostream &os, const Command &rhs);
  friend istream &operator>>(istream &is, Command &rhs);
};

class CommandInventory;

class CommandInventoryFactory : public CommandFactory {
public:
  CommandInventoryFactory();
  Command *create() const override;
};

class CommandInventory : public Command {
public:
  bool execute(HashTable<Customer *> &customers,
               map<string, vector<Movie *>> &movies) override;

private:
  ostream &display(ostream &os) const override;
  istream &input(istream &is) override;
};

class CommandHistory;

class CommandHistoryFactory : public CommandFactory {
public:
  CommandHistoryFactory();
  Command *create() const override;
};

class CommandHistory : public Command {
public:
  bool execute(HashTable<Customer *> &customers,
               map<string, vector<Movie *>> &movies) override;
  // getters
  int getID() const;
  // setters
  void setID(const int &id);

private:
  int id = 0;

  ostream &display(ostream &os) const override;
  istream &input(istream &is) override;
};

class CommandBorrow;

class CommandBorrowFactory : public CommandFactory {
public:
  CommandBorrowFactory();
  Command *create() const override;
};

class CommandBorrow : public Command {
public:
  bool execute(HashTable<Customer *> &customers,
               map<string, vector<Movie *>> &movies) override;
  // getters
  int getID() const;
  string getMedia() const;
  string getGenre() const;
  string getSortingAttributes() const;
  // setters
  void setID(const int &id);
  void setMedia(const string &genre);
  void setGenre(const string &genre);
  void setSortingAttributes(const string &sortingAttributes);

private:
  int id = 0;
  string media{"D"};
  string genre;
  string sortingAttributes;
  ostream &display(ostream &os) const override;
  istream &input(istream &is) override;
};

class CommandReturn;

class CommandReturnFactory : public CommandFactory {
public:
  CommandReturnFactory();
  Command *create() const override;
};

class CommandReturn : public Command {
public:
  bool execute(HashTable<Customer *> &customers,
               map<string, vector<Movie *>> &movies) override;
  // getters
  int getID() const;
  string getMedia() const;
  string getGenre() const;
  string getSortingAttributes() const;
  // setters
  void setID(const int &id);
  void setMedia(const string &genre);
  void setGenre(const string &genre);
  void setSortingAttributes(const string &sortingAttributes);

private:
  int id = 0;
  string media{"D"};
  string genre;
  string sortingAttributes;

  ostream &display(
      ostream &os) const override;
  istream &input(istream &is) override;
};

#endif