#include "tester.h"
#include "Reggie/nfa_eval.h"
#include "Reggie/nfa.h"
#include "Reggie/nfa_eval.h"
#include "Reggie/nfa_create.h"

////
// set up an NFA for the regex aba

NFA aba_nfa() {
	NFAMold mold = nfa_mold_create();
	unsigned int q0 = nfa_mold_initialState(mold);
	unsigned int q1 = nfa_mold_addState(mold);
	unsigned int q2 = nfa_mold_addState(mold);
	unsigned int q3 = nfa_mold_addState(mold);
	nfa_mold_makeStateTerminal(mold, q3);

	nfa_mold_addTransition(mold, q0, "a", q1);
	nfa_mold_addTransition(mold, q1, "b", q2);
	nfa_mold_addTransition(mold, q2, "a", q3);

	NFA nfa = nfa_mold_compile(mold);
	nfa_mold_destroy(mold);

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
	state_setID(q2, 2);
	State q3 = state_create();
	state_setID(q3, 3);
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
	state_setID(q2, 2);
	State q3 = state_create();
	state_setID(q3, 3);
	state_makeTerminal(q3);

	state_addTransition(q2, "a", q3);

	passed = is_false(state_eval(q2, "aa"));

	state_destroy(q2);
	state_destroy(q3);

	return (result){passed, description};
}

result test_state_eval_aa_PassingWithEpsilonTransition() {
	int passed = 0;
	char* description = "state_eval(State, char*) : \"aa\" passing with epsilon transition CASE";

	State q1 = state_create();
	state_setID(q1, 1);
	State q2 = state_create();
	state_setID(q2, 2);
	State q3 = state_create();
	state_setID(q3, 3);
	State q4 = state_create();
	state_setID(q4, 4);
	state_makeTerminal(q4);

	state_addTransition(q1, "a", q2);
	state_addTransition(q2, "", q3);
	state_addTransition(q3, "a", q4);

	passed = is_true(state_eval(q1, "aa"));

	state_destroy(q1);
	state_destroy(q2);
	state_destroy(q3);
	state_destroy(q4);

	return (result){passed, description};
}

result test_state_eval_a_PassingWithFirstTransition() {
	int passed = 0;
	char* description = "state_eval(State, char*) : \"a\" passing with second transition CASE";

	State q2 = state_create();
	state_setID(q2, 2);
	State q3 = state_create();
	state_setID(q3, 3);
	state_makeTerminal(q3);

	state_addTransition(q2, "a", q3);
	state_addTransition(q2, "b", q3);

	passed = is_true(state_eval(q2, "a"));

	state_destroy(q2);
	state_destroy(q3);

	return (result){passed, description};
}

result test_state_eval_a_PassingWithSecondTransition() {
	int passed = 0;
	char* description = "state_eval(State, char*) : \"a\" passing with second transition CASE";

	State q2 = state_create();
	state_setID(q2, 2);
	State q3 = state_create();
	state_setID(q3, 3);
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
	state_setID(q2, 2);
	State q3 = state_create();
	state_setID(q3, 3);
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

result test_nfa_eval_ABA_PASSINGWithEpsilons() {
	int passed = 0;
	char* description = "nfa_eval(NFA, char*) : aba passing with epsilons CASE";

	NFAMold mold = nfa_mold_create();
	unsigned int states[8];
	states[0] = nfa_mold_initialState(mold);
	for (int i = 1; i < 8; i++) {
		states[i] = nfa_mold_addState(mold);
	}

	nfa_mold_addTransition(mold, states[0], "", states[1]);
	nfa_mold_addTransition(mold, states[1], "", states[2]);
	nfa_mold_addTransition(mold, states[2], "", states[3]);
	nfa_mold_addTransition(mold, states[3], "a", states[4]);
	nfa_mold_addTransition(mold, states[4], "b", states[5]);
	nfa_mold_addTransition(mold, states[5], "a", states[6]);
	nfa_mold_makeStateTerminal(mold, states[6]);

	NFA nfa = nfa_mold_compile(mold);
	nfa_mold_destroy(mold);

	passed = is_true(nfa_eval(nfa, "aba"));

	nfa_destroy(nfa);

	return (result){passed, description};
}

result test_nfa_eval_aUb_PASSINGWithEpsilons() {
	int passed = 0;
	char* description = "nfa_eval(NFA, char*) : a|b passing with epsilons CASE";

	NFAMold mold = nfa_mold_create();
	unsigned int q0 = nfa_mold_initialState(mold);
	unsigned int q1 = nfa_mold_addState(mold);
	unsigned int q2 = nfa_mold_addState(mold);
	unsigned int q3 = nfa_mold_addState(mold);
	unsigned int q4 = nfa_mold_addState(mold);

	nfa_mold_addTransition(mold, q0, "", q1);
	nfa_mold_addTransition(mold, q0, "", q2);
	nfa_mold_addTransition(mold, q1, "a", q3);
	nfa_mold_addTransition(mold, q2, "b", q4);
	nfa_mold_makeStateTerminal(mold, q3);
	nfa_mold_makeStateTerminal(mold, q4);

	NFA nfa = nfa_mold_compile(mold);
	nfa_mold_destroy(mold);

	passed = is_true(nfa_eval(nfa, "a")) && is_true(nfa_eval(nfa, "b"));

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
	&test_state_eval_aa_PassingWithEpsilonTransition,
	&test_nfa_eval_aUb_PASSINGWithEpsilons,
	&test_state_eval_a_PassingWithSecondTransition,
	&test_state_eval_aa_FailingWithSecondTransition,

	// test nfa_eval()
	&test_nfa_eval_ABA_PASSING,
	&test_nfa_eval_ABA_PASSINGWithEpsilons,
	&test_nfa_eval_ABA_Failing,
	
	0
};

int main(int argc, char** argv) {
	run_tests(TESTS);
	return 0;
}