#include "tester.h"
#include <Reggie/nfa_eval.h>
#include <Reggie/nfa.h>

////
// set up an NFA for the regex aba

NFA aba_nfa() {
	NFA nfa = nfa_create();
	state_id q0 = nfa_initialState(nfa);
	state_id q1 = nfa_addState(nfa);
	state_id q2 = nfa_addState(nfa);
	state_id q3 = nfa_addState(nfa);
	nfa_state_makeTerminal(nfa, q3);

	nfa_addTransition(nfa, nfa_initialState(nfa), q1, "a");
	nfa_addTransition(nfa, q1, q2, "b");
	nfa_addTransition(nfa, q2, q3, "a");

	return nfa;
}

////
// test int state_parsing_eval(State, char*)

result test_state_parsing_eval_Empty_PASSING() {
	int passed = 0;
	char* description = "state_parsing_eval(State, char*) : \"\" passing terminal CASE";

	NFA s = nfa_create();
	nfa_state_makeTerminal(nfa_initialState(s));

	passed = is_nonfalse(nfa_parsing_eval(s, ""));

	nfa_destroy(s);

	return (result){passed, description};
}

result test_state_parsing_eval_s_PassingOnEmptyStringTerminal() {
	int passed = 0;
	char* description = "state_parsing_eval(State, char*) : \"s\" passing on empty string terminal CASE";

	NFA s = nfa_create();
	nfa_state_makeTerminal(nfa_initialState(s));

	passed = is_nonfalse(nfa_parsing_eval(s, "s"));

	nfa_destroy(s);

	return (result){passed, description};
}

result test_state_parsing_eval_EmptyString_FailingNonTerminal() {
	int passed = 0;
	char* description = "state_parsing_eval(State, char*) : \"\" failing non-terminal CASE";

	NFA s = nfa_create();

	passed = is_false(nfa_parsing_eval(s, ""));

	nfa_destroy(s);

	return (result){passed, description};
}

result test_state_parsing_eval_a_PassingWithTransition() {
	int passed = 0;
	char* description = "state_parsing_eval(State, char*) : \"a\" passing with transition CASE";

	NFA n = nfa_create();
	state_id s0 = nfa_initialState(n);
	state_id s1 = nfa_addState(n);
	nfa_addTransition(n, s0, s1, "a");
	nfa_state_makeTerminal(n, s1);

	passed = is_nonfalse(nfa_parsing_eval(n, "a"));

	nfa_destroy(n);

	return (result){passed, description};
}

result test_state_parsing_eval_aa_PassingLenght1WithTransition() {
	int passed = 0;
	char* description = "state_parsing_eval(State, char*) : \"aa\" passing length 1 with transition CASE";

	NFA n = nfa_create();
	state_id s0 = nfa_initialState(n);
	state_id s1 = nfa_addState(n);
	nfa_addTransition(n, s0, s1, "a");
	nfa_state_makeTerminal(n, s1);

	passed = is_nonfalse(nfa_parsing_eval(n, "aa"));

	nfa_destroy(n);

	return (result){passed, description};
}

result test_state_parsing_eval_a_PassingWithSecondTransition() {
	int passed = 0;
	char* description = "state_parsing_eval(State, char*) : \"a\" passing with second transition CASE";

	NFA n = nfa_create();
	state_id s0 = nfa_initialState(n);
	state_id s1 = nfa_addState(n);
	nfa_addTransition(n, s0, s1, "a");
	nfa_addTransition(n, s0, s1, "b");
	nfa_state_makeTerminal(n, s1);

	passed = is_nonfalse(nfa_parsing_eval(n, "a"));

	nfa_destroy(n);

	return (result){passed, description};
}

result test_state_parsing_eval_aa_PassingWithEpsilonTransition() {
	int passed = 0;
	char* description = "state_parsing_eval(State, char*) : \"aa\" passing with epsilon transition CASE";

	NFA n = nfa_create();
	state_id s0 = nfa_initialState(n);
	state_id s1 = nfa_addState(n);
	state_id s2 = nfa_addState(n);
	state_id s3 = nfa_addState(n);
	nfa_state_makeTerminal(n, s3);
	nfa_addTransition(n, s0, s1, "a");
	nfa_addTransition(n, s1, s2, "");
	nfa_addTransition(n, s2, s3, "a");

	passed = is_nonfalse(nfa_parsing_eval(q1, "aa"));

	nfa_destroy(n);

	return (result){passed, description};
}

result test_state_parsing_eval_aa_FailingWithSecondTransition() {
	int passed = 0;
	char* description = "state_parsing_eval(State, char*) : \"aa\" failing with second transition CASE";

	NFA n = nfa_create();
	state_id s0 = nfa_initialState(n);
	state_id s1 = nfa_addState(n);
	nfa_addTransition(n, s0, s1, "b");
	nfa_addTransition(n, s0, s1, "c");

	passed = is_false(state_parsing_eval(n, "aa"));

	nfa_destroy(n);

	return (result){passed, description};
}

////
// test int nfa_parsing_eval(NFA, char*)

result test_nfa_parsing_eval_ABA_PASSING() {
	int passed = 0;
	char* description = "nfa_parsing_eval(NFA, char*) : aba passing CASE";

	NFA nfa = aba_nfa();

	passed = is_nonfalse(nfa_parsing_eval(nfa, "aba"));

	nfa_destroy(nfa);

	return (result){passed, description};
}

result test_nfa_parsing_eval_ABAAAA_PassingLength3() {
	int passed = 0;
	char* description = "nfa_parsing_eval(NFA, char*) : abaaaa passing length 3 CASE";

	NFA nfa = aba_nfa();

	passed = is_equal(nfa_parsing_eval(nfa, "abaaaa"), 3);

	nfa_destroy(nfa);

	return (result){passed, description};
}

result test_nfa_parsing_eval_ABA_Failing() {
	int passed = 0;
	char* description = "nfa_parsing_eval(NFA, char*) : aba failing CASE";

	NFA nfa = aba_nfa();

	passed = is_false(nfa_parsing_eval(nfa, "aaa"));

	nfa_destroy(nfa);

	return (result){passed, description};
}

////
// main function and test list definition

static tests TESTS = {
	// test state_parsing_eval() (also tests transition_eval() by proxy)
	&test_state_parsing_eval_Empty_PASSING,
	&test_state_parsing_eval_s_PassingOnEmptyStringTerminal,
	&test_state_parsing_eval_EmptyString_FailingNonTerminal,
	&test_state_parsing_eval_a_PassingWithTransition,
	&test_state_parsing_eval_aa_PassingLenght1WithTransition,
	&test_state_parsing_eval_a_PassingWithSecondTransition,
	&test_state_parsing_eval_aa_FailingWithSecondTransition,

	// test nfa_parsing_eval()
	&test_nfa_parsing_eval_ABA_PASSING,
	&test_nfa_parsing_eval_ABAAAA_PassingLength3,
	&test_nfa_parsing_eval_ABA_Failing,
	
	0
};

int main(int argc, char** argv) {
	return run_tests(TESTS);
}