#include <Reggie/nfa_copy.h>
#include <Collection/list_sort.h>

int state_sorter(state a, state b) {
	if (a->id <= b->id)
		return 1;
	return 0;
}

// create a copy of an nfa from a given nfa
NFA nfa_copy(NFA nfa) {
	NFA copy = nfa_create();
	list_destroy(nfa->automata.states);
	nfa->automata.states = list_create();
	nfa->automata.next_state_id = 0;

	// for each state in the nfa, copy it
	// make copies of each transition and attach them to the correct copied states
	list sortedStates = list_sort(nfa->automata.states, (sort_fn)&state_sorter);

	FOREACH(it, sortedStates) {
		state s = VALUE(it);
		if (s != initial) {
			unsigned int i = nfa_addState(copy);
			if (s->isTerminal) nfa_state_makeTerminal(copy, s);
		}
	}

	FOREACH(it, sortedStates) {
		state s = VALUE(it);
		FOREACH(t_it, s->transitions) {
			transition t = VALUE(t_it);
			NFATransition nfat = container_of(t, struct nfa_transition, transition);
			state d = t->dest;
			nfa_addTransition(mold, s->id, d->id, nfat->transition_string);
		}
	}

	list_destroy(sortedStates);
	return copy;
}
