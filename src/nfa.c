#include <stdlib.h>
#include "nfa.h"
#include "strings.h"

NFA nfa_create() {
	NFA nfa = calloc(1, sizeof(struct nfa));
	nfa->states = list_create();
	nfa->initialState = state_create();
	state_setID(nfa->initialState, 0);
	nfa_addState(nfa, nfa_initialState(nfa));
	return nfa;
}

void nfa_destroy(NFA nfa) {
	state_destroy(nfa_initialState(nfa));
	// FOREACH(it, nfa->states) {
	// 	state_destroy(VALUE(it));
	// }
	list_destroy(nfa->states);
	free(nfa);
}

State nfa_initialState(NFA nfa) {
	return nfa->initialState;
}

void nfa_setInitialState(NFA nfa, State s) {
	if (nfa->initialState) {
		list_removeValue(nfa->states, nfa->initialState);
		state_destroy(nfa->initialState);
	}
	nfa->initialState = s;
}

void nfa_addState(NFA nfa, State s) {
	list_push(nfa->states, s);
}

State state_create() {
	State s = calloc(1, sizeof(struct state));
	s->transitions = list_create();
	state_setID(s, 0);
	state_makeNonTerminal(s);
	return s;
}

void state_setID(State s, unsigned int id) {
	s->id = id;
}

unsigned int state_getID(State s) {
	return s->id;
}

void state_setName(State s, char* name) {
	if (s->name != 0)
		free(s->name);
	s->name = string_copy(name);
}

char *state_getName(State s) {
	return s->name;
}

void state_makeTerminal(State s) {
	s->isTerminalState = 1;
}

void state_makeNonTerminal(State s) {
	s->isTerminalState = 0;
}

void state_destroy(State s) {
	FOREACH(it, s->transitions) {
        Transition t = VALUE(it);
        free(t->transition_string);
        free(t);
	}
	list_destroy(s->transitions);
	free(s);
}

void state_addTransition(State s, char* transition_string, State dest) {
	Transition t = calloc(1, sizeof(struct transition));
	t->transition_string = string_copy(transition_string);
	t->source = s;
	t->dest = dest;
	list_push(s->transitions, t);
}

struct state_removeTransition_find_func_state {
	char* transition_string;
	State dest;
};

// this is the find function needed to iterate
// through each transition and find the one to remove
int state_removeTransition_find_func(void* state, void* value) {
	struct state_removeTransition_find_func_state* s = (struct state_removeTransition_find_func_state*)state;
	Transition t = (Transition)value;
	if (s->dest == t->dest) {
		return string_equals(s->transition_string, t->transition_string);
	}
	return 0;
}

void state_removeTransition(State s, char* transition_string, State dest) {
	// make a state structure to pass through to the find_func
	struct state_removeTransition_find_func_state state = (struct state_removeTransition_find_func_state){transition_string, dest};

	index_value_pair result = list_find(s->transitions, &state_removeTransition_find_func, &state);
	int index = result.index;
	Transition t = result.value;

	free(t->transition_string);
	free(t);

	list_removeFrom(s->transitions, index);
}

#include <stdio.h>

void nfa_print(NFA nfa) {
	if (nfa_initialState(nfa)->name == 0)
		printf("Intital: %d\n", nfa_initialState(nfa)->id);
	else
		printf("Intital: %s\n", nfa_initialState(nfa)->name);
	list_foreach(nfa->states, (foreach_func)&state_print);
}

void state_print(State s) {
	char* terminal = s->isTerminalState ? "!" : "";
	if (s->name != 0) {
		printf("%s-%d:\t%s\n", s->name, s->id, terminal);
	} else {
		printf("%d:\t%s\n", s->id, terminal);
	}
	list_foreach(s->transitions, (foreach_func)&transition_print);
}

void transition_print(Transition t) {
	if (t->source->name == 0) {
		if (t->dest->name == 0) {
			printf("\t%d --\"%s\"--> %d\n", t->source->id, t->transition_string, t->dest->id);
		} else {
			printf("\t%d --\"%s\"--> %s\n", t->source->id, t->transition_string, t->dest->name);
		}
	} else {
		if (t->dest->name == 0) {
			printf("\t%s --\"%s\"--> %d\n", t->source->name, t->transition_string, t->dest->id);
		} else {
			printf("\t%s --\"%s\"--> %s\n", t->source->name, t->transition_string, t->dest->name);
		}
	}
}