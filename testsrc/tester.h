#ifndef TESTER_H
#define TESTER_H

#include <stdio.h>

// a data structure to return when testing
// functions complete.
typedef struct {
	int passed; // 1 if the test passed, 0 otherwise
	char* description; // a description of the test
} result;

// the signature of testign functions
typedef result (*testing_func)();

// short-hand for a c-array of testing functions
typedef testing_func tests[];

// execute a list of tests and print out how many passed
// out of how many total tests were run from each testing function.
// the tests passed in must be NULL-terminated.
int run_tests(tests ts);

// a more readable way of writing x == 1
int is_true(int value);

// a more readable way of writing x == 0
int is_false(int value);

// a more readable way of writing x != 0
int is_nonfalse(int value);

// a more readable way of writing x == 0
int is_zero(int value);

// a more readable way of writing x != 0
int is_nonzero(int value);

// a more readable way of writing x == y
int is_equal(int x, int y);

//////
// TEST WRITING

// used to get the function name of a test
#define TEST(name) _test_ ## name

// begin test code with the given name and description
#define TEST_BEGIN(name, description) TEST(name) () { char *__desc = "TEST: " #name "; " #description "" ;

// end test code with the given exit status
#define TEST_END(passed) return (result){passed, __desc}; }

//////
// IMPLEMENTATION

// execute a list of tests and print out how many passed
// out of how many total tests were run from each testing function.
// the tests passed in must be NULL-terminated.
inline int run_tests(tests ts) {
	int test_counter = 0;
	int passed_counter = 0;
	for (int i = 0; ts[i] != 0; i++) {
		result r = ts[i]();

		printf("%s : TEST %s\n", (r.passed ? "PASSED" : "FAILED"), r.description);

		test_counter++;
		if (r.passed) passed_counter++;
	}

	// unnecessary ternary statement, but i was 
	// really in a ternary statement kind of mood 
	printf("ALL TESTS: %s\n", (test_counter == passed_counter ? "PASSED" : ""));
	if (test_counter != passed_counter) {
		printf("\tPASSED:\t%d\n", passed_counter);
		printf("\tFAILED:\t%d\n", test_counter - passed_counter);
		printf("\tTOTAL:\t%d\n", test_counter);
		return -1;
	}
	return 0;
}

// a more readable way of writing x == 1
inline int is_true(int value) {
	return !!value == 1;
}

// a more readable way of writing x == 0
inline int is_false(int value) {
	return value == 0;
}

// a more readable way of writing x != 0
inline int is_nonfalse(int value) {
	return value != 0;
}

// a more readable way of writing x == 0
inline int is_zero(int value) {
	return value == 0;
}

// a more readable way of writing x != 0
inline int is_nonzero(int value) {
	return value != 0;
}

// a more readable way of writing x == y
inline int is_equal(int x, int y) {
	return x == y;
}

#endif
