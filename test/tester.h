#ifndef TESTER_H
#define TESTER_H

// a data structure to return when testing
// functions complete.
typedef struct {
	int pass; // the number of tests within the function that passed
	int total; // the total number of tests within the function
	char* name; // the name of the test resulting from the function
} result;

// the signature of testign functions
typedef result (*testing_func)();

// short-hand for a c-array of testing functions
typedef testing_func* tests;

// execute a list of tests and print out how many passed
// out of how many total tests were run from each testing function.
// the tests passed in must be NULL-terminated.
void run_tests(tests ts);

// a more readable way of writing x == 1
int is_true(int value);

// a more readable way of writing x == 0
int is_false(int value);

// a more readable way of writing x == y
int is_equal(int x, int y);

#end