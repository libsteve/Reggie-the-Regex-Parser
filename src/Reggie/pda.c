#include "Reggie/pda.h"
#include <stdlib.h>

PDA pda_create() {
	PDA pda = calloc(1, sizeof(struct pda));
	pda->states = list_create();
	pda->transitions = list_create();
	pda->stack = list_create();
	return pda;
}

void pda_destroy(PDA pda) {
	FOREACH(it, pda->states) {
		free(VALUE(it));
	}
	FOREACH(it, pda->transitions) {
		free(VALUE(it));
	}
	list_destroy(pda->states);
	list_destroy(pda->transitions);
	list_destroy(pda->stack);
	free(pda);
}

int pda_addState(PDA pda) {
	PDAState s = calloc(1, sizeof(struct pda_state));
	s->id = list_len(pda->states);
	s->isTerminal = 0;
	s->transitions = list_create();
	list_push(pda->states, s);
	return s->id;
}

PDAState pda_getState(PDA pda, int state) {
	FOREACH(it, pda->states) {
		PDAState s = VALUE(it);
		if (s->id == state) {
			return s;
		}
	}
	return NULL;
}

int pda_state_isTerminal(PDA pda, int state) {
	PDAState s = pda_getState(pda, state);
	if (s && s->isTerminal) {
		return 1;
	}
	return 0;
}

void pda_state_makeTerminal(PDA pda, int state) {
	PDAState s = pda_getState(pda, state);
	if (s) {
		s->isTerminal = 1;
	}
}

void pda_state_makeNonTerminal(PDA pda, int state) {
	PDAState s = pda_getState(pda, state);
	if (s) {
		s->isTerminal = 0;
	}
}

PDATransition pda_getTransition(PDA pda, int transition) {
	FOREACH(it, pda->transitions) {
		PDATransition t = VALUE(it);
		if (t->id == transition) {
			return t;
		}
	}
	return NULL;
}

int pda_addTransition(PDA pda, int src, int dest, pda_transition_fn func) {
	PDATransition t = calloc(1, sizeof(struct pda_transition));
	t->id = list_len(pda->transitions);
	t->source = src;
	t->dest = dest;
	t->function = func;
	list_push(pda->transitions, t);
	list_push(pda_getState(pda, src)->transitions, t);
	return t->id;
}

void pda_removeTransition(PDA pda, int transition) {
	PDATransition t = pda_getTransition(pda, transition);
	list_removeValue(pda->transitions, t);
	PDAState s = pda_getState(pda, t->source);
	if (s) {
		list_removeValue(s->transitions, t);
	}
	free(t);
}

list pda_stack(PDA pda) {
	return pda->stack;
}