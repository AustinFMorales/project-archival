/**
 * Testing ass4 movie store functions
 *
 * @author Yusuf Pisan
 * @date 19 Jan 2019
 */

#include "store.h"
#include <cassert>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>

using namespace std;

void testStoreFinal() {
  cout << "=====================================" << endl;
  cout << "Start testStoreFinal" << endl;

  Store blockbuster;

  blockbuster.readMovies("data4movies.txt");
  blockbuster.readCustomers("data4customers.txt");
  blockbuster.readCommands("data4commands.txt");

  assert(blockbuster.getCustomer(4444) != nullptr);
  assert(blockbuster.getCustomer(1997) == nullptr);
  assert(blockbuster.getCustomer(1111) != nullptr);
  assert(blockbuster.getCustomer(0042) == nullptr);

  blockbuster.executeCommands();

  assert(blockbuster.getCustomer(8000)->getHistory().size() == 7);
  assert(blockbuster.getCustomer(5000)->getHistory().size() == 9);
  assert(blockbuster.getCustomer(1000)->getHistory().size() == 7);
  assert(blockbuster.getCustomer(1111)->getHistory().size() == 5);
  cout << "End testStoreFinal" << endl;
  cout << "=====================================" << endl;
}

void testAll() { testStoreFinal(); }
