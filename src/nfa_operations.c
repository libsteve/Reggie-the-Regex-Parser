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
// creates an epsilon transition between a start state and a destination state
// makes the start state non-terminal
// persistantState - the state for all transitions to lead to
// variableState - the state for transitions to originate from
// this conforms to the foreachWithState_func
void __state_makeEpsilonTransition(void* persistantState, void* variableState) {
	// make an epsilon transition from the terminalState to b's initial
	state_addTransition((State)variableState, "", (State)persistantState);

	// this state is no longer a terminal state
	state_makeNonTerminal((State)variableState);

	state_print((State)variableState);
}

#include <stdio.h>

NFA nfa_CONCAT(NFA a, NFA b) {
	list terminalStates = nfa_getTerminalStates(a);
	list_foreach(terminalStates, (foreach_func)&state_print);
	list_foreachWithState(terminalStates, &__state_makeEpsilonTransition, nfa_initialState(b));

	list allStates = list_merge(a->states, b->states);
	State initialState = nfa_initialState(a);

	a->states = list_create(); // we need a list int the NFA to safely destroy
	b->states = list_create(); // we need a list int the NFA to safely destroy
	nfa_destroy(a);
	nfa_destroy(b);

	NFA nfa = nfa_create();
	list_destroy(nfa->states); // destroy what is given
	state_destroy(nfa_initialState(nfa)); // destroy what is given

	nfa->states = allStates; // use what we have
	nfa->initialState = initialState; // use what we have

	list_destroy(terminalStates);

	nfa_print(nfa);

	return nfa;
}

NFA nfa_UNION(NFA a, NFA b) {
	NFA nfa = nfa_create();

	state_addTransition(nfa_initialState(nfa), "", nfa_initialState(a));
	state_addTransition(nfa_initialState(nfa), "", nfa_initialState(b));

	nfa->states = list_merge(list_merge(nfa->states, a->states), b->states);

	a->states = list_create(); // we need a list int the NFA to safely destroy
	b->states = list_create(); // we need a list int the NFA to safely destroy
	nfa_destroy(a);
	nfa_destroy(b);

	return nfa;
}

NFA nfa_KLEENE(NFA a) {
	NFA nfa = nfa_create();
	state_makeTerminal(nfa_initialState(nfa));

	list terminalStates = nfa_getTerminalStates(a);
	list_foreachWithState(terminalStates, &__state_makeEpsilonTransition, nfa_initialState(nfa));

	list_rpush(a->states, list_pop(nfa->states));

	list temp = nfa->states;
	nfa->states = a->states;
	a->states = temp;

	nfa_destroy(a);

	return nfa;
}