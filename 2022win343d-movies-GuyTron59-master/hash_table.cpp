#ifndef HASH_TABLE_CPP
#define HASH_TABLE_CPP
#include "hash_table.h"

template <class T> HashTable<T>::HashTable() { container.resize(buckets); }

template <class T> HashTable<T>::~HashTable() {
  for (auto v : container) {
    for (auto p : v) {
      delete p.second;
    }
    v.clear();
  }

  container.clear();
}

template <class T> int HashTable<T>::hashFunction(int key) const {
  return key % buckets;
}

template <class T> bool HashTable<T>::empty() const {
  return container.size() == 0;
}

template <class T> bool HashTable<T>::add(int key, T &val) {
  if (count(key) == 0) {
    container[hashFunction(key)].push_back(make_pair(key, new T(val)));
    return true;
  }
  return false;
}

template <class T> int HashTable<T>::count(int key) const {
  int hashKey = hashFunction(key);
  for (auto p : container[hashKey]) {
    if (p.first == key) {
      return 1;
    }
  }

  return 0;
}

template <class T> T &HashTable<T>::operator[](const int &key) {
  int hashKey = hashFunction(key);
  for (auto p : container[hashKey]) {
    if (p.first == key) {
      return *(p.second);
    }
  }
  return *(container[hashKey][container[hashKey].size() - 1].second);
}
#endif