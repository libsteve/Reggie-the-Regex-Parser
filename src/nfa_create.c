#include "nfa_create.h"
#include "strings.h"
#include <stdlib.h>

NFAMold nfa_mold_create() {
	NFAMold mold = calloc(1, sizeof(struct nfa_mold));
	mold->states = list_create();
	mold->transitions = list_create();
	mold->state_count = 0;
	nfa_mold_createState(mold);
	return mold;
}

void nfa_mold_destroy(NFAMold mold) {
	FOREACH(it, mold->states) {
		free(VALUE(it));
	}
	list_destroy(mold->states);
	FOREACH(it, mold->transitions) {
		TransitionMold t = VALUE(it);
		free(t->string);
		free(t);
	}
	list_destroy(mold->transitions);
	free(mold);
}

unsigned int nfa_mold_createState(NFAMold mold) {
	unsigned int id = mold->state_count;
	mold->state_count = id + 1;
	StateMold s = calloc(1, sizeof(struct state_mold));
	s->id = id;
	s->isTerminal = 0;
	list_push(mold->states, s);
	return id;
}

// private function
// get the specific state mold
StateMold nfa_mold_findState(NFAMold mold, unsigned int id) {
	StateMold s = 0;
	FOREACH(it, mold->states) {
		StateMold sm = VALUE(it);
		if (sm->id == id) {
			s = sm;
			break;
		}
	}
	return s;
}

int nfa_mold_isStateTerminal(NFAMold mold, unsigned int id) {
	StateMold s = nfa_mold_findState(mold, id);
	if (s != 0) {
		return s->isTerminal;
	}
	return 0;
}

void nfa_mold_makeStateTerminal(NFAMold mold, unsigned int id) {
	StateMold s = nfa_mold_findState(mold, id);
	if (s != 0) {
		s->isTerminal = 1;
	}
}

void nfa_mold_makeStateNonTerminal(NFAMold mold, unsigned int id) {
	StateMold s = nfa_mold_findState(mold, id);
	if (s != 0) {
		s->isTerminal = 0;
	}
}

void nfa_mold_addTransition(NFAMold mold, unsigned int s1, char *string, unsigned int s2) {
	StateMold s = nfa_mold_findState(mold, s1);
	StateMold d = nfa_mold_findState(mold, s2);
	if (s && d) {
		TransitionMold t = calloc(1, sizeof(struct transition_mold));
		t->string = string_copy(string);
		t->source = s1;
		t->dest = s2;
		list_push(mold->transitions, t);
	}
}

void nfa_mold_removeTransition(NFAMold mold, unsigned int s1, char *string, unsigned int s2) {
	TransitionMold t = 0;
	FOREACH(it, mold->transitions) {
		TransitionMold tm = VALUE(it);
		if (string_equals(tm->string, string)) {
			t = tm;
			break;
		}
	}
	if (t != 0) {
		list_removeValue(mold->transitions, t);
	}
}

// private function
State find_state(list states, unsigned int id) {
	FOREACH(it, states) {
		State s = VALUE(it);
		if (s->id == id)
			return s;
	}
	return 0;
}

NFA nfa_mold_compile(NFAMold mold) {
	list states = list_create();
	FOREACH(it, mold->states) {
		StateMold sm = VALUE(it);
		State s = state_create();
		state_setID(s, sm->id);
		if (sm->isTerminal) {
			state_makeTerminal(s);
		}
		list_push(states, s);
		// printf("createing state #%d\n", sm->id);
	}
	FOREACH(it, mold->transitions) {
		TransitionMold tm = VALUE(it);
		State a = find_state(states, tm->source);
		State b = find_state(states, tm->dest);
		state_addTransition(a, tm->string, b);
		// printf("addign state %d to %d over %s", tm->source, tm->dest, tm->string);
	}
	NFA nfa = nfa_create();
	FOREACH(it, states) {
		State s = VALUE(it);
		if (s->id == 0) {
			nfa_setInitialState(nfa, s);
		}
		nfa_addState(nfa, s);
		// printf("adding state #%d\n", s->id);
	}
	list_destroy(states);
	return nfa;
}