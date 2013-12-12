#include "tester.h"
#include <Reggie/nfa.h>
#include <Collection/strings.h>

//
// i got lazy about testing this... most of it is REALLY easy...
//

result test_nfa_addState() {
	int passed = 0;
	char* description = "CASE : nfa_addState()";

	NFA n = nfa_create();
	nfa_addState(n);

	passed = is_true(list_len(n->automata.states) == 2);

	nfa_destroy(n);

	return (result){passed, description};
}

result test_nfa_addTransition() {
	int passed = 0;
	char* description = "CASE : nfa_addTransition() with \"abc\"";

	NFA n = nfa_create();
	state_id q0 = nfa_initialState(n);
	state_id q1 = nfa_addState(n);
	transition_id t1 = nfa_addTransition(n, q0, q1, "abc"); 

	passed = is_true(	list_len(n->automata.states) == 2
					&&	list_len(n->automata.transitions) == 1
					);

	NFATransition t = container_of(list_getFrom(n->automata.transitions, 0), struct nfa_transition, transition);

	passed = is_true(	passed
					&&	t->transition.id == t1
					&&	string_equals("abc", t->transition_string)
					);

	NFAState s0 = container_of(list_getFrom(n->automata.states, 0), struct nfa_state, state);
	if (s0->state.id != q0) {
		s0 = container_of(list_getFrom(n->automata.states, 1), struct nfa_state, state);
	}

	passed = is_true(	passed
					&&	list_len(s0->state.transitions) == 1
					&&	((transition)list_getFrom(s0->state.transitions, 0))->id == t1
					);

	nfa_destroy(n);

	return (result){passed, description};
}

////
// test 
//
// result test_() {
// 	int passed = 0;
// 	char* description = "";
//
//	
//
// 	passed = ;
//
//	
//
// 	return (result){passed, description};
// }

////
// main function and test list definition

static tests TESTS = {
	test_nfa_addState,
	test_nfa_addTransition,
	0
};

int main(int argc, char** argv) {
	return run_tests(TESTS);
}
