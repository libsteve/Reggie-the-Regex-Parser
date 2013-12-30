#include <Reggie/nfa_copy.h>
#include <Reggie/automata_copy.h>
#include <Collection/list_sort.h>

automata nfa_copy_create() {
	NFA n = nfa_create();
	return &n->automata;
}

state_id nfa_copy_initialState(automata a) {
	NFA n = container_of(a, struct nfa, automata);
	return nfa_initialState(n);
}

state_id nfa_copy_copyState(automata destination, state s) {
	NFA nfa_dst = container_of(destination, struct nfa, automata);
	//NFAState nfa_state = container_of(s, struct nfa_state, state);
	state_id new = nfa_addState(nfa_dst);
	if (s->isTerminal) {
		nfa_state_makeTerminal(nfa_dst, new);
	} else {
		nfa_state_makeNonTerminal(nfa_dst, new);
	}
	return new;
}

transition_id nfa_copy_copyTransition(automata dest, transition t, state_id newSrc, state_id newDst) {
	NFA nfa = container_of(dest, struct nfa, automata);
	NFATransition nfa_t = container_of(t, struct nfa_transition, transition);
	transition_id new = nfa_addTransition(nfa, newSrc, newDst, nfa_t->transition_string);
	return new;
}

// create a copy of an nfa from a given nfa
NFA nfa_copy(NFA nfa) {
	automata a = automata_copy(&nfa->automata, (automata_copy_creation){nfa_copy_create, nfa_copy_initialState, nfa_copy_copyState, nfa_copy_copyTransition});
	return container_of(a, struct nfa, automata);
}

