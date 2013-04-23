#include "nfa_copy-test.h"
#include "../src/nfa.h"
#include "../src/nfa_eval.h"

////
// set up an NFA for the regex aba

NFA aba_nfa() {
	NFA nfa = nfa_create();

	State q1 = state_create();
	state_setName(q1, "q1");

	State q2 = state_create();
	state_setName(q2, "q2");

	State q3 = state_create();
	state_setName(q3, "q3");
	state_makeTerminal(q3);

	state_addTransition(nfa_initialState(nfa), "a", q1);
	state_addTransition(q1, "b", q2);
	state_addTransition(q2, "a", q3);

	return nfa;
}

NFA aUb_nfa() {
	NFA nfa = nfa_create();
	State q1 = state_create();
	State q2 = state_create();
	State q3 = state_create();
	State q4 = state_create();

	nfa_addState(nfa, q1);
	nfa_addState(nfa, q2);
	nfa_addState(nfa, q3);
	nfa_addState(nfa, q4);

	state_addTransition(nfa_initialState(nfa), "", q1);
	state_addTransition(nfa_initialState(nfa), "", q2);
	state_addTransition(q1, "a", q3);
	state_addTransition(q2, "b", q4);
	state_makeTerminal(q3);
	state_makeTerminal(q4);

	return nfa;
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
	run_tests(TESTS);
	return 0;
}