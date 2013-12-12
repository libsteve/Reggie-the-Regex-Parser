#include "tester.h"
#include <Reggie/nfa.h>
#include <Reggie/nfa_copy.h>
#include <Reggie/nfa_eval.h>

////
// set up an NFA for the regex aba

NFA aba_nfa() {
	NFA mold = nfa_create();
	state_id a = nfa_addState(mold);
	state_id b = nfa_addState(mold);
	state_id c = nfa_addState(mold);
	nfa_state_makeTerminal(mold, c);

	nfa_addTransition(mold, nfa_initialState(mold), a, "a");
	nfa_addTransition(mold, a, b, "b");
	nfa_addTransition(mold, b, c, "a");

	return mold;
}

NFA aUb_nfa() {
	NFA mold = nfa_create();
	state_id a = nfa_addState(mold);
	state_id b = nfa_addState(mold);
	state_id c = nfa_addState(mold);
	state_id d = nfa_addState(mold);
	nfa_state_makeTerminal(mold, c);
	nfa_state_makeTerminal(mold, d);

	nfa_addTransition(mold, nfa_initialState(mold), "", a);
	nfa_addTransition(mold, nfa_initialState(mold), "", b);
	nfa_addTransition(mold, a, "b", c);
	nfa_addTransition(mold, b, "b", d);
	nfa_addTransition(mold, b, "a", c);

	return mold;
}

////
// test 

result test_nfa_copy_1() {
	int passed = 0;
	char* description = "nfa_copy() : CASE \"aba\"";

	NFA first = aba_nfa();
	NFA second = nfa_copy(first);
	nfa_destroy(first);

	passed = is_true(nfa_eval(second, "aba") && 
					!nfa_eval(second, "a"));
	nfa_destroy(second);

	return (result){passed, description};
}

result test_nfa_copy_2() {
	int passed = 0;
	char* description = "nfa_copy() : CASE \"a|b\"";

	NFA first = aUb_nfa();
	NFA second = nfa_copy(first);
	nfa_destroy(first);

	passed = is_true(nfa_eval(second, "a") && 
					 nfa_eval(second, "b") &&
					 !nfa_eval(second, ""));
	nfa_destroy(second);

	return (result){passed, description};
}

////
// main function and test list definition

static tests TESTS = {
	test_nfa_copy_1,
	test_nfa_copy_2,
	
	0
};

int main(int argc, char** argv) {
	return run_tests(TESTS);
}
