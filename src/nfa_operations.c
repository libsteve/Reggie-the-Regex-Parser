#include "nfa_operations.h"
#include "list.h"

// private
// for list nfa_getTerminalStates(NFA)
// perform this function on every state in an NFA
// if the state is a terminal state, add it to the list of terminal states
void nfa_getTerminalStates_foreachWithState_func(void* state, void* value) {
	State s = (State)value;
	list terminalStates = (list)state;

	if (s->isTerminalState)
		list_push(terminalStates, s);
}

// private
// get the terminal states from an NFA
list nfa_getTerminalStates(NFA nfa) {
	list l = list_create();
	list_foreachWithState(nfa->states, &nfa_getTerminalStates_foreachWithState_func, l);
	return l;
}

// private
// for NFA nfa_CONCAT(NFA, NFA)
// perform this function on every transition state found in a
void nfa_CONCAT_foreachWithState_func(void* state, void* value) {
	State b_initialState = (State)state;
	State terminalState = (State)value;

	// make an epsilon transition from the terminalState to b's initial
	state_addTransition(terminalState, "", b_initialState);

	// this state is no longer a terminal state
	state_makeNonTerminal(terminalState);
}

NFA nfa_CONCAT(NFA a, NFA b) {
	list terminalStates = nfa_getTerminalStates(a);
	list_foreachWithState(terminalStates, &nfa_CONCAT_foreachWithState_func, nfa_initialState(b));

	list allStates = list_merge(a->states, b->states);
	State initialState = nfa_initialState(a);

	a->states = list_create(); // we need a list here to safely destroy
	b->states = list_create(); // we need a list here to safely destroy
	nfa_destroy(a);
	nfa_destroy(b);

	NFA nfa = nfa_create();
	list_destroy(nfa->states); // destroy what is given
	state_destroy(nfa_initialState(nfa)); // destroy what is given

	nfa->states = allStates; // use what we have
	nfa->initialState = initialState; // use what we have
}

NFA nfa_UNION(NFA a, NFA b) {

}

NFA nfa_KLEENE(NFA a) {

}