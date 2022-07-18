#include "command.h"
#include "customer.h"

map<string, CommandFactory *> &Command::getMap() {
  static map<string, CommandFactory *> factories;
  return factories;
}

void Command::registerType(const string &type, CommandFactory *factory) {
  getMap().emplace(type, factory);
}

Command *Command::create(const string &type) {
  if (getMap().count(type) == 0) {
    return nullptr;
  }

  return getMap().at(type)->create();
}

bool Command::hasExecuted() { return executed; }

ostream &operator<<(ostream &os, const Command &rhs) { return rhs.display(os); }

istream &operator>>(istream &is, Command &rhs) { return rhs.input(is); }

CommandInventoryFactory::CommandInventoryFactory() {
  Command::registerType("I", this);
}

Command *CommandInventoryFactory::create() const {
  return new CommandInventory();
}

bool CommandInventory::execute(HashTable<Customer *> & /*customers*/,
                               map<string, vector<Movie *>> &movies) {
  for (const auto &p : movies) {
    cout << p.first << endl;
    for (Movie *m : p.second) {
      cout << *m << endl;
    }
    cout << endl;
  }

  executed = true;
  return true;
}

ostream &CommandInventory::display(ostream &os) const {
  os << "I";
  return os;
} // no need because does not have any private data members

istream &CommandInventory::input(istream &is) {
  return is;
} // no need because does not have any private data members

CommandInventoryFactory anonymousCommandInventoryFactory;

CommandHistoryFactory::CommandHistoryFactory() {
  Command::registerType("H", this);
}

Command *CommandHistoryFactory::create() const { return new CommandHistory(); }

bool CommandHistory::execute(HashTable<Customer *> &customers,
                             map<string, vector<Movie *>> & /*movies*/) {
  /* if (customers.count(id) > 0) {
    for (Movie *m : customers[id]->getBorrowed()) {
      cout << *m << endl;
    }
  } */

  if (customers.count(id) > 0) {
    // cout << *(customers[id]) << endl;
    // add check history (this) to customer history as well
    // cout << *this << endl;
    customers[id]->addToHistory(this);
    customers[id]->printHistory();
    executed = true;
    return true;
  }

  return false;
}

ostream &CommandHistory::display(ostream &os) const {
  os << "H"
     << " " << id;
  return os;
}

istream &CommandHistory::input(istream &is) {
  is >> id;
  return is;
}

CommandHistoryFactory anonymousCommandHistoryFactory;

CommandBorrowFactory::CommandBorrowFactory() {
  Command::registerType("B", this);
}

Command *CommandBorrowFactory::create() const { return new CommandBorrow(); }

// making execute return bool that identifies whether it succeeded or failed
bool CommandBorrow::execute(HashTable<Customer *> &customers,
                            map<string, vector<Movie *>> &movies) {
  // need cerr
  Customer *c = nullptr;
  executed = true;

  if (customers.count(id) > 0) {
    c = customers[id];
  } else {
    return false;
  }

  Movie *mo = Movie::create(genre), *target = nullptr;

  if (mo == nullptr) {
    cerr << "INVALID GENRE GIVEN FOR BORROW" << endl;
    return false;
  }

  mo->setUsingSortingAttributes(sortingAttributes);
  mo->setMedia(media);

  for (Movie *m : movies[genre]) {
    if (*m == *mo) {
      target = m;
      break;
    }
  }

  if (target != nullptr && target->getStock() > 0) {
    c->borrowMovie(target);
    customers[id]->addToHistory(this);
    return true;
  }

  cerr << "MOVIE NOT FOUND" << endl;
  return false;
}

ostream &CommandBorrow::display(ostream &os) const {
  os << "B"
     << " " << id << " " << media << " " << genre << " " << sortingAttributes;
  return os;
}

istream &CommandBorrow::input(istream &is) {
  is >> id >> media >> genre;
  is.get();
  getline(is, sortingAttributes);
  return is;
}

CommandBorrowFactory anonymousCommandBorrowFactory;

CommandReturnFactory::CommandReturnFactory() {
  Command::registerType("R", this);
}

Command *CommandReturnFactory::create() const { return new CommandReturn(); }

bool CommandReturn::execute(HashTable<Customer *> &customers,
                            map<string, vector<Movie *>> &movies) {
  Customer *c = nullptr;

  if (customers.count(id) > 0) {
    c = customers[id];
  } else {
    return false;
  }

  Movie *mo = Movie::create(genre), *target = nullptr;

  if (mo == nullptr) {
    cerr << "INVALID GENRE GIVEN FOR BORROW" << endl;
    return false;
  }

  mo->setUsingSortingAttributes(sortingAttributes);
  mo->setMedia(media);

  for (Movie *m : movies[genre]) {
    if (*m == *mo) {
      target = m;
      break;
    }
  }

  if (target != nullptr && target->getStock() > 0) {
    if (c->returnMovie(target)) {
      customers[id]->addToHistory(this);
      return true;
    }
  }
  executed = true;

  return false;
}

ostream &CommandReturn::display(ostream &os) const {
  os << "R"
     << " " << id << " " << media << " " << genre << " " << sortingAttributes;
  return os;
}

istream &CommandReturn::input(istream &is) {
  is >> id >> media >> genre;
  is.get();
  getline(is, sortingAttributes);
  return is;
}

CommandReturnFactory anonymousCommandReturnFactory;