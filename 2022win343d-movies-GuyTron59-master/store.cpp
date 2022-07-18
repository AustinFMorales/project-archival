#include "store.h"

Store::Store() {
  movies["F"];
  movies["D"];
  movies["C"];
}

Store::~Store() {
  for (auto p : movies) {
    for (Movie *m : p.second) {
      delete m;
    }

    p.second.clear();
  }

  movies.clear();
}

bool Store::readMovies(const string &fileName) {
  ifstream ifs(fileName);
  if (!ifs.good()) {
    cerr << "Failed to open: " << fileName << endl;
    return false;
  }

  string line, type;
  while (getline(ifs, line)) {
    istringstream ss(line);

    if (movies.count(type = static_cast<char>(ss.get())) == 1 &&
        ss.get() == ',') {
      Movie *m = Movie::create(type);
      ss >> *m;
      movies[type].push_back(m);
      sort(movies[type].begin(), movies[type].end());
    } else if (!line.empty()) {
      cerr << "ERROR: Unknown genre " << type << endl;
    }
  }

  return true;
}

void Store::printInventory() {
  for (const auto &p : movies) {
    cout << p.first << endl;
    for (Movie *m : p.second) {
      cout << *m << endl;
    }
    cout << endl;
  }
}

bool Store::readCustomers(const string &fileName) {
  ifstream ifs(fileName);
  if (!ifs.good()) {
    cerr << "Failed to open: " << fileName << endl;
    return false;
  }

  string line;
  while (getline(ifs, line)) {
    istringstream ss(line);

    if (!line.empty()) {
      auto c = new Customer();
      ss >> *c;
      customers.add(c->getID(), c);
    }
  }

  return true;
}

bool Store::readCommands(const string &fileName) {
  ifstream ifs(fileName);
  if (!ifs.good()) {
    cerr << "Failed to open: " << fileName << endl;
    return false;
  }

  string line, type;
  while (getline(ifs, line)) {
    istringstream ss(line);
    ss >> type;
    ss.get();
    Command *c = Command::create(type);
    if (c != nullptr) {
      ss >> *c;
      commands.push(c);
    } else {
      cerr << "INVALID TYPE " << type << endl;
    }
  }
  return true;
}

void Store::executeCommands() {
  while (!commands.empty()) {
    if (!commands.front()->execute(customers, movies)) {
      delete commands.front();
    }
    commands.pop();
  }
}

Customer *Store::getCustomer(const int &id) {
  return customers.count(id) > 0 ? customers[id] : nullptr;
}