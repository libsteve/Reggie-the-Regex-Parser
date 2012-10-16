#include "nfa_eval-test.h"
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

	state_addTransition(nfa_initialState(), "a", q1);
	state_addTransition(q1, "b", q2);
	state_addTransition(q2, "a", q3);

	return nfa;
}

////
// test int state_eval(State, char*)

result test_state_eval_Empty_PASSING() {
	int passed = 0;
	char* description = "state_eval(State, char*) : \"\" passing terminal CASE";

	State s = state_create();
	state_makeTerminal(s);

	passed = is_true(state_eval(s, ""));

	state_destroy(s);

	return (result){passed, description};
}

result test_state_eval_s_Failing() {
	int passed = 0;
	char* description = "state_eval(State, char*) : \"s\" failing terminal CASE";

	State s = state_create();
	state_makeTerminal(s);

	passed = is_false(state_eval(s, "s"));

	state_destroy(s);

	return (result){passed, description};
}

result test_state_eval_EmptyString_FailingNonTerminal() {
	int passed = 0;
	char* description = "state_eval(State, char*) : \"\" failing non-terminal CASE";

	State s = state_create();

	passed = is_false(state_eval(s, ""));

	state_destroy(s);

	return (result){passed, description};
}

result test_state_eval_a_PassingWithTransition() {
	int passed = 0;
	char* description = "state_eval(State, char*) : \"a\" passing with transition CASE";

	State q2 = state_create();
	state_setName(q2, "q2");
	State q3 = state_create();
	state_setName(q3, "q3");
	state_makeTerminal(q3);

	state_addTransition(q2, "a", q3);

	passed = is_true(state_eval(q2, "a"));

	state_destroy(q2);
	state_destroy(q3);

	return (result){passed, description};
}

result test_state_eval_aa_FailingWithTransition() {
	int passed = 0;
	char* description = "state_eval(State, char*) : \"aa\" failing with transition CASE";

	State q2 = state_create();
	state_setName(q2, "q2");
	State q3 = state_create();
	state_setName(q3, "q3");
	state_makeTerminal(q3);

	state_addTransition(q2, "a", q3);

	passed = is_false(state_eval(q2, "aa"));

	state_destroy(q2);
	state_destroy(q3);

	return (result){passed, description};
}

result test_state_eval_a_PassingWithSecondTransition() {
	int passed = 0;
	char* description = "state_eval(State, char*) : \"a\" passing with second transition CASE";

	State q2 = state_create();
	state_setName(q2, "q2");
	State q3 = state_create();
	state_setName(q3, "q3");
	state_makeTerminal(q3);

	state_addTransition(q2, "b", q3);
	state_addTransition(q2, "a", q3);

	passed = is_true(state_eval(q2, "a"));

	state_destroy(q2);
	state_destroy(q3);

	return (result){passed, description};
}

result test_state_eval_aa_FailingWithSecondTransition() {
	int passed = 0;
	char* description = "state_eval(State, char*) : \"aa\" failing with second transition CASE";

	State q2 = state_create();
	state_setName(q2, "q2");
	State q3 = state_create();
	state_setName(q3, "q3");
	state_makeTerminal(q3);

	state_addTransition(q2, "b", q3);
	state_addTransition(q2, "a", q3);

	passed = is_false(state_eval(q2, "aa"));

	state_destroy(q2);
	state_destroy(q3);

	return (result){passed, description};
}

////
// test int nfa_eval(NFA, char*)

result test_nfa_eval_ABA_PASSING() {
	int passed = 0;
	char* description = "nfa_eval(NFA, char*) : aba passing CASE";

	NFA nfa = aba_nfa();

	passed = is_true(nfa_eval(nfa, "aba"));

	nfa_destroy(nfa);

	return (result){passed, description};
}

result test_nfa_eval_ABA_Failing() {
	int passed = 0;
	char* description = "nfa_eval(NFA, char*) : aba failing CASE";

	NFA nfa = aba_nfa();

	passed = is_false(nfa_eval(nfa, "aaa"));

	nfa_destroy(nfa);

	return (result){passed, description};
}

////
// main function and test list definition

static tests TESTS = {
	// test state_eval() (also tests transition_eval() by proxy)
	&test_state_eval_Empty_PASSING,
	&test_state_eval_s_Failing,
	&test_state_eval_EmptyString_FailingNonTerminal,
	&test_state_eval_a_PassingWithTransition,
	&test_state_eval_aa_FailingWithTransition,
	&test_state_eval_a_PassingWithSecondTransition,
	&test_state_eval_aa_FailingWithSecondTransition,

	// test nfa_eval()
	&test_nfa_eval_ABA_PASSING,
	&test_nfa_eval_ABA_Failing,
	
	0
};

int main(int argc, char** argv) {
	run_tests(TESTS);
	return 0;
}