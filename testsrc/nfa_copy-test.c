#include "tester.h"
#include "Reggie/nfa.h"
#include "Reggie/nfa_copy.h"
#include "Reggie/nfa_eval.h"
#include "Reggie/nfa_create.h"

////
// set up an NFA for the regex aba

NFA aba_nfa() {
	NFAMold mold = nfa_mold_create();
	unsigned int a = nfa_mold_addState(mold);
	unsigned int b = nfa_mold_addState(mold);
	unsigned int c = nfa_mold_addState(mold);
	nfa_mold_makeStateTerminal(mold, c);

	nfa_mold_addTransition(mold, nfa_mold_initialState(mold), "a", a);
	nfa_mold_addTransition(mold, a, "b", b);
	nfa_mold_addTransition(mold, b, "a", c);

	NFA nfa = nfa_mold_compile(mold);
	nfa_mold_destroy(mold);

	return nfa;
}

NFA aUb_nfa() {
	NFAMold mold = nfa_mold_create();
	unsigned int a = nfa_mold_addState(mold);
	unsigned int b = nfa_mold_addState(mold);
	unsigned int c = nfa_mold_addState(mold);
	unsigned int d = nfa_mold_addState(mold);
	nfa_mold_makeStateTerminal(mold, c);
	nfa_mold_makeStateTerminal(mold, d);

	nfa_mold_addTransition(mold, nfa_mold_initialState(mold), "", a);
	nfa_mold_addTransition(mold, nfa_mold_initialState(mold), "", b);
	nfa_mold_addTransition(mold, a, "b", c);
	nfa_mold_addTransition(mold, b, "b", d);
	nfa_mold_addTransition(mold, b, "a", c);

	NFA nfa = nfa_mold_compile(mold);
	nfa_mold_destroy(mold);

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