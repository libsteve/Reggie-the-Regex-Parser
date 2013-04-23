#include "nfa_copy.h"
#include "strings.h"
#include <stdlib.h>

// create an nfa template from a given nfa
NFATemplate nfa_template_create(NFA nfa) {
	NFATemplate t = calloc(1, sizeof(struct nfa_template));
	t->states = map_create();
	t->transitions = map_create();
	t->initialState = nfa_initialState(nfa);
	nfa_template_addStateRecursive(t, t->initialState);
	return t;
}

// destroy a template nfa instance
void nfa_template_destroy(NFATemplate t) {
	FOREACH_ENTRY(ent, t->states) {
		state_template_destroy(ENTRY_VALUE(ent));
	}
	FOREACH_ENTRY(ent, t->transitions) {
		transition_template_destroy(ENTRY_VALUE(ent));
	}
	map_destroy(t->states);
	map_destroy(t->transitions);
	free(t);
}

// create a state template from a given state
StateTemplate state_template_create(State s) {
	StateTemplate st = calloc(1, sizeof(struct state_template));
	st->isTerminal = s->isTerminalState;
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

	NFA nfa = nfa_create();
	state_destroy(nfa_initialState(nfa));

	// create all new states
	FOREACH_ENTRY(ent, t->states) {
		State s = state_create();
		StateTemplate st = ENTRY_VALUE(ent);
		state_setName(s, st->name);
		nfa_addState(nfa, s);		
		if (st->isTerminal) {
			state_makeTerminal(s);
		} else {
			state_makeNonTerminal(s);
		}
		if (t->initialState == ENTRY_KEY(ent)) {
			nfa->initialState = s;
		}
		map_add(result_states, ENTRY_KEY(ent), s);
	}

	// add in all transitions
	FOREACH_ENTRY(ent, t->transitions) {
		TransitionTemplate tt = ENTRY_VALUE(ent);
		State s1 = map_get(result_states, tt->source);
		State s2 = map_get(result_states, tt->dest);
		char *str = tt->string;
		state_addTransition(s1, str, s2);
	}

	map_destroy(result_states);

	return nfa;
}

// create a copy of an nfa from a given nfa
NFA nfa_copy(NFA nfa) {
	NFATemplate t = nfa_template_create(nfa);
	NFA result = nfa_createFromTemplate(t);
	nfa_template_destroy(t);
	return result;
}