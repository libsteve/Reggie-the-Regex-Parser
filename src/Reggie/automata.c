#include <Reggie/automata.h>

transition transition_initialize(transition t, state src, state dest, transition_func func, transition_uninitialize uninitialize) {
	t->id = 0;
	t->src = src;
	t->dst = dst;
	t->func = func;
	t->uninitialize = uninitialize;
	return t;
}

void transition_uninitialize(transition t) {
	t->src = NULL;
	t->dst = NULL;
}

state state_initialize(state s, bool isTerminal, state_uninitialize uninitialize) {
	s->id = 0;
	s->transitions = list_create();
	s->isTerminal = isTerminal;
	s->uninitialize = uninitialize;
	return s;
}

void state_uninitialize(state s) {
	list_destroy(s->transitions);
	s->transitions = NULL;
}

automata automata_initialize(automata a) {
	a->states = list_create();
	a->transitions = list_create();
	a->next_state_id = 0;
	a->next_transition_id = 0;
	return a;
}

void automata_uninitialize(automata a) {
	FOREACH(it, a->states) {
		state s = VALUE(it);
		s->uninitialize(s);
	}
	FOREACH(it, a->transitions) {
		transition t = VALUE(it);
		t->uninitialize(t);
	}
	list_destroy(a->states);
	list_destroy(a->transitions);
	a->states = NULL;
	a->transitions = NULL;
}

state_id automata_addState(automata a, state s) {
	s->id = a->next_state_id;
	a->next_state_id += 1;
	return s->id;
}

transition_id automata_addTransition(autamata a, transition t) {
	t->id = a->next_transition_id;
	a->next_transition_id += 1;
	return t->id;
}
