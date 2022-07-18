#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <functional>
#include <iostream>
#include <utility>
#include <vector>
using namespace std;

// use pointers here, so we can actually verify
// if stuff is uninitialized
// try to avoid overwriting values in this hashtable implementation
template <class T> class HashTable {
private:
  // store our simple HashTable implementation in a vector of vectors
  // closed hashing
  vector<vector<pair<int, T *>>> container;
  // follow pisan's advice in class in regards of the amount of buckets
  int buckets = 26;

public:
  HashTable();
  ~HashTable();

  // functions that are not allowed to get rid of
  // clang-tidy errors
  // copy constructor not allowed
  HashTable(const HashTable &other) = delete;
  // move not allowed
  HashTable(HashTable &&other) = delete;
  // assignment operator and move assignment not allowed
  HashTable &operator=(const HashTable &other) = delete;
  HashTable &operator=(const HashTable &&other) = delete;

  // simple hashfunction to store indexes quick
  int hashFunction(int key) const;

  // add value to hashtable with int key and t value
  // represent a pair
  bool add(int key, T &val);

  // similar to STL count in maps and sets
  // return 1 the key is not nullptr and it exists
  // return 0 if otherwise
  int count(int key) const;

  // check if its empty
  bool empty() const;

  // take the operator[] overload to allow array subscript operations
  // similar to STL map
  T &operator[](const int &key);
};

// need this for templates to prevent undefined reference
#include "hash_table.cpp"
#endif