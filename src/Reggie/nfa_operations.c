#include <Reggie/nfa_operations.h>
#include <Reggie/automata_operations.h>

automata nfa_automata_create() {
	NFA nfa = nfa_create();
	return nfa->automata;
}

state_id nfa_automata_addState(automata a) {
	NFA nfa = container_of(a, struct nfa, automata);
	return nfa_addState(nfa);
}

transition_id nfa_automata_addEpislonTransition(automata a, state_id src, state_id dest) {
	NFA nfa = container_of(a, struct nfa, automata);
	return nfa_addTransition(nfa, src, dest, "");
}

transition_id nfa_automata_copyTransition(automata destination, transition t, state_id newSrcm state_id newDest) {
	NFA nfa = container_of(a, struct nfa, automata);
	NFATransition t_nfa = container_of(a, struct nfa_transition, transition);
	return nfa_addTransition(nfa, src, dest, t_nfa->transition_string);
}

automata_creation nfa_creation = { 
	.create = nfa_automata_create,
	.addState = nfa_automata_addState,
	.addEpislonTransition = nfa_automata_addEpislonTransition,
	.copyTransition = nfa_automata_copyTransition
};

NFA nfa_CONCAT(NFA a, NFA b) {
	return container_of(automata_concat(&a->automata, &b->automata, nfa_creation), struct nfa, automata);
}

NFA nfa_UNION(NFA a, NFA b) {
	return container_of(automata_union(&a->automata, &b->automata, nfa_creation), struct nfa, automata);
}

NFA nfa_KLEENE(NFA a) {
	return container_of(automata_kleene(&a->automata, nfa_creation), struct nfa, automata);
}

NFA nfa_PLUS(NFA a) {
	return container_of(automata_plus(&a->automata, nfa_creation), struct nfa, automata);
}