#include "nfa_copy.h"
#include "strings.h"
#include <stdlib.h>

// create an nfa template from a given nfa
NFATemplate nfa_template_create(NFA nfa) {
	NFATemplate t = calloc(1, sizeof(struct nfa_template));
	t->states = map_create();
	nfa_template_addStateRecursive(t, nfa_initialState(nfa));
	return t;
}

// destroy a template nfa instance
void nfa_template_destroy(NFATemplate t) {
	FOREACH_ENTRY(ent, t->states) {
		state_template_destroy(ENTRY_VALUE(ent));
	}
	map_destroy(t->states);
	free(t);
}

// create a state template from a given state
StateTemplate state_template_create(State s) {
	StateTemplate st = calloc(1, sizeof(struct state_template));
	st->terminal = s->isTerminalState;
	st->name = string_copy(s->name);
	return st;
}

// destroy a state template nfa instance
void state_template_destroy(StateTemplate st) {
	free(st->name);
	free(st);
}

// add a state, all it's transitions, and all the connected states to the template
void nfa_template_addStateRecursive(NFATemplate t, State s) {
	if (map_has(t->states, s))
		return;

	StateTemplate st = state_template_create(s);
	map_set(t->states, s, st);
	FOREACH(it, s->transitions) {
		Transition tr = VALUE(it);
		nfa_template_addTransitionRecursive(t, tr);
	}
}

// create a transition template from a given transition
TransitionTemplate transition_template_create(Transition t) {
	TransitionTemplate tt = calloc(1, sizeof(struct transition_template));
	tt->string = string_copy(t->transition_string);
	tt->source = t->source;
	tt->dest = t->dest;
	return tt;
}

// destroy a transition template instance
void transition_template_destroy(TransitionTemplate tt) {
	free(tt->string);
	free(tt);
}

// add a transition and all the connected states to the template
void nfa_template_addTransitionRecursive(NFATemplate t, Transition tr) {
	if (map_has(t->transitions, tr))
		return;

	TransitionTemplate tt = transition_template_create(tr);
	map_set(t->transitions, t, tt);
	nfa_template_addStateRecursive(t, tt->source);
	nfa_template_addStateRecursive(t, tt->dest);
}

// create an nfa from a given nfa template
NFA nfa_createFromTemplate(NFATemplate t) {
	map result_states = map_create();

	// TODO: finish this function

	map_destroy(result_states);
}

// create a copy of an nfa from a given nfa
NFA nfa_copy(NFA nfa) {
	NFATemplate t = nfa_template_create(nfa);
	NFA result = nfa_createFromTemplate();
	nfa_template_destroy(t);
	return result;
}