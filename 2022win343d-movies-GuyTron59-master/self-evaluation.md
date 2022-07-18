# Self-Evaluation

## Name(s): Austin Ferdinand Morales and Guy Hayardeny

Out of 25 points. Use output.txt created using 
`./create-output.sh > output.txt 2>&1` for guidance.

Complete all questions with "Q:"

Q: Does the program compile and run to completion: Yes

- If the program does not compile or gives a segmentation error when run, 
the maximum possible grade is 50%. No need to continue with the rest of self-evaluation

Q: All public functions have been implemented: 0

- -2 for each functionality not implemented

For each command, state Full, Partial or None to indicate 
if it has been fully, partially or not implemented at all.
Explain any partial implementations.

Inventory: Full
History: Full
Borrow: Full
Return: Full


Q: -1 for each compilation warning, min -3: 0

- Check under *1. Compiles without warnings*
- If the warning message is addressed in README.md, including how the programmer tried to address it, no deductions

Q: -1 for each clang-tidy warning, min -3: 0 (Addressed in README.md becasue using self registering factories)

- Check under *3. clang-tidy warnings*
- If the warning message is addressed in README.md, including how the programmer tried to address it, no deductions

Q: -1 for each clang-format warning, min -3: 0

- Check under *4. clang-format does not find any formatting issues*


Q: -2 for any detected memory leak: 0 (we tried fixing the memory leaks we had but were unable because of issues with double pointers and destructors)

- Check under *5. No memory leaks using g++*
- Check under *6. No memory leaks using valgrind*

Q: Do the tests sufficiently test the code: ENTER_NUMBER

- -1 for each large block of code not executed
- -2 for each function that is never called when testing
- Check under *7. Tests have full code coverage* paying attention to *The lines below were never executed*

Q: Are all functions in .h and .cpp file documents (min -3): 0

- -1 for each function not documented

## Location of error message or functionality

State the file and function where the information can be found

invalid command code: Command class

invalid movie type: Command class and Movie class

invalid customer ID: Command class, Store, and HashTable
 
invalid movie: Movie class

factory classes: Movie class (drama, classic, comedy) & Command class  (CommandInventory, CommandHistory, CommandBorrow, and CommandReturn)

hashtable: Hash table is used for storing the int key id with the customer object

container used for comedy movies: STL map<string, vector<Movie*>> called movieDatabase. comedy is stored in movieDatabase["F"]

function for sorting comedy movies: Store class in processMovies using std::sort() after each insertion

function where comedy movies are sorted: processMovies using std::sort after each insertion

functions called when retrieving a comedy movie based on title and year: CommandBorrow and CommandReturn classes through execute() function

functions called for retrieving and printing customer history: CommandHistory and CommandInventory through execute function

container used for customer history: vector<Command*> in Customer class

functions called when borrowing a movie: CommandBorrow execute();

explain borrowing a movie that does not exist: Printing out cerr statements in CommandBorrow if we don't find an equivalent movie using a temp movie pointers

explain borrowing a movie that has 0 stock: Returning false in execute for not having enough stock for a certain movie

explain returning a movie that customer has not checked out: check the customer's borrowed vector using returnMovie in CommandReturn, if it returns false, then we do not return the movie they haven't checked out at all

any static_cast or dynamic_cast used: dynamic_cast used in operator overloads for Movie class, DramaMovie class, ClassicMovie class, and ComedyMovie class


Q: Total points: ADD_ALL_POINTS (max 25): 25