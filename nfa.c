#include "nfa.h"
#include "strings.h"

NFA nfa_create() {
	NFA nfa = calloc(1, sizeof(struct nfa));
	nfa->states = list_create();
	nfa->initialState = state_create();
	state_setName(nfa->initialState, "q0");
	list_push(nfa->states, nfa->initialState);
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

State state_create() {
	State s = calloc(1, sizeof(struct state));
	s->transitions = list_create();
	state_setName(s, "q0");
	state_makeNonTerminal(s);
	return s;
}

void state_setName(State s, char* name) {
	s->name;
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
	t->destination = dest;
	list_push(s->transitions, t);
}

struct state_removeTransition_find_func_state {
	char* transition_string;
	State dest;
};

// this is the find function needed to iterate
// through each transition and find the one to remove
int state_removeTransition_find_func(void* state, void* value) {
	if (state->dest == value->dest) {
		return string_equals(state->transition_string, value->transition_string);
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