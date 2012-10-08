#include "tester.h"
#include <stdio.h>

void run_tests(tests ts) {
	int test_counter = 0;
	int passed_counter = 0;
	for (int i = 0; ts[i] != 0; i++) {
		result r = ts[i]();

		printf("%s : TEST %s", (r.passed ? "PASSED" : "FAILED"), r.description);

		test_counter++;
		if (r.passed) passed_counter++;
	}

	// unnecessary ternary statement, but i was 
	// really in a ternary statement kind of mood 
	printf("ALL TESTS: %s\n", (test_counter == passed_counter ? "PASSED" : ""));
	if (test_counter != passed_counter) {
		printf("\tPASSED:\t%d\n", passed_counter);
		printf("\tFAILED:\t%s\n", test_counter - passed_counter);
		printf("\tTOTAL:\t%d\n", test_counter);
	}
}

// a more readable way of writing x == 1
int is_true(int value) {
	return value == 1;
}

// a more readable way of writing x == 0
int is_false(int value) {
	return value == 0;
}

// a more readable way of writing x == y
int is_equal(int x, int y) {
	return x == y;
}