#include "reggie-test.h"

////
// test ReggieRegex reggie_create(char*)

result test_reggie_create() {
	int passed = 0;
	char* description = "reggie_create() : CASE (a|b)*";

	ReggieRegex rr = reggie_create("(a|b)*");
	if (rr == 0) {
		passed = 0;
	} else {
		passed = is_true(reggie_match(rr, "a") &&
						 reggie_match(rr, "b") &&
						 reggie_match(rr, "") &&
						 reggie_match(rr, "aab") &&
						 reggie_match(rr, "aba"));
	}

	reggie_destroy(rr);

	return (result){passed, description};
}

////
// main function and test list definition

static tests TESTS = {
	&test_reggie_create,

	0
};

int main(int argc, char** argv) {
	run_tests(TESTS);
	return 0;
}