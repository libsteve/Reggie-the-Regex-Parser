#include "list_sort.h"
#include "nfa_copy.h"
#include "strings.h"

int state_sorter(State a, State b) {
	if (state_getID(a) <= state_getID(b))
		return 1;
	return 0;
}

// generate an nfa mold from a given nfa
NFAMold nfa_mold_createFromNFA(NFA nfa) {
	NFAMold mold = nfa_mold_create();
	// copy the initial state
	// for each state in the nfa that isn't the initial state, copy it
	// make copies of each transition and attach them to the correct copies states
	State initial = nfa_initialState(nfa);
	list sortedStates = list_sort(nfa->states, (sort_fn)&state_sorter);
	if (state_isTerminal(initial)) {
		nfa_mold_makeStateTerminal(mold, nfa_mold_initialState(mold));
	}

	FOREACH(it, sortedStates) {
		State s = VALUE(it);
		if (s != initial) {
			unsigned int i = nfa_mold_addState(mold);
			if (state_isTerminal(s))
				nfa_mold_makeStateTerminal(mold, i);
		}
	}

	FOREACH(it, sortedStates) {
		State s = VALUE(it);
		FOREACH(t_it, s->transitions) {
			Transition t = VALUE(t_it);
			State d = t->dest;
			nfa_mold_addTransition(mold, state_getID(s), t->transition_string, state_getID(d));
		}
	}

	list_destroy(sortedStates);
	return mold;
}

// create a copy of an nfa from a given nfa
NFA nfa_copy(NFA nfa) {
	NFAMold mold = nfa_mold_createFromNFA(nfa);
	NFA copy = nfa_createFromMold(mold);
	nfa_mold_destroy(mold);
	return copy;
}