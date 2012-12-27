#include <stdlib.h>
#include "nfa.h"
#include "strings.h"

NFA nfa_create() {
	NFA nfa = calloc(1, sizeof(struct nfa));
	nfa->states = list_create();
	nfa->initialState = state_create();
	state_setName(nfa->initialState, "q0");
	nfa_addState(nfa, nfa_initialState(nfa));
	return nfa;
}

// this is the foreach function needed to iterate
// through each state and destroy it when
// used within the nfa_destroy() function
void nfa_destroy_foreach_func(void* value) {
	state_destroy(value);
}

void nfa_destroy(NFA nfa) {
	list_foreach(nfa->states, &nfa_destroy_foreach_func);
	list_destroy(nfa->states);
	free(nfa);
}

State nfa_initialState(NFA nfa) {
	return nfa->initialState;
}

void nfa_addState(NFA nfa, State s) {
	list_push(nfa->states, s);
}

State state_create() {
	State s = calloc(1, sizeof(struct state));
	s->transitions = list_create();
	state_setName(s, "q0");
	state_makeNonTerminal(s);
	return s;
}

void state_setName(State s, char* name) {
	s->name = name;
}

void state_makeTerminal(State s) {
	s->isTerminalState = 1;
}

void state_makeNonTerminal(State s) {
	s->isTerminalState = 0;
}

// this is the foreach function needed to iterate
// through each transition and delete it when
// used within the state_destroy() function
void state_destroy_foreach_func(void* value) {
	free(value);
}

void state_destroy(State s) {
	list_foreach(s->transitions, &state_destroy_foreach_func);
	list_destroy(s->transitions);
	free(s);
}

void state_addTransition(State s, char* transition_string, State dest) {
	Transition t = calloc(1, sizeof(struct transition));
	t->transition_string = transition_string;
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

	list_removeFrom(s->transitions, index);
}

#include <stdio.h>

void nfa_print(NFA nfa) {
	printf("Intital: %s\n", nfa_initialState(nfa)->name);
	list_foreach(nfa->states, (foreach_func)&state_print);
}

void state_print(State s) {
	char* terminal = s->isTerminalState ? "!" : "";
	printf("%s:\t%s\n", s->name, terminal);
	list_foreach(s->transitions, (foreach_func)&transition_print);
}

void transition_print(Transition t) {
	printf("\t%s --\"%s\"--> %s\n", t->source->name, t->transition_string, t->dest->name);
}