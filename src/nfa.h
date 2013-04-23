#ifndef NFA_H
#define NFA_H

#include "list.h"

// a transition to a state
typedef struct transition {
	char* transition_string; // a string to transition through when encountered
							 // this must be an allocated string
	struct state* source; // the source state for the transition
	struct state* dest; // the destination state for the transition
} *Transition;

// a state in an nfa
typedef struct state {
	int isTerminalState; // 1 if the state is terminal, 0 otherwise
	char* name; // the name of the state
				// this must be an allocated string
	list transitions; // a list of transitions that originate from this state
} *State;

// a non-deterministic finite automata
typedef struct nfa {
	struct state* initialState; // the initial state for the nfa
	list states; // a list of states that exist within the nfa
} *NFA;

// create a new nfa object
// all nfa's begin with an initial state named "q0"
// "q0" is initially a non-terminal state with no transitions
NFA nfa_create();

// destroy an nfa object and all states within
// use this instead of free
void nfa_destroy(NFA nfa);

// get the initial state of the NFA
State nfa_initialState(NFA nfa);

// add a state to the NFA's state list
void nfa_addState(NFA nfa, State s);

// create a new state object
State state_create();

// give a name to a state
// you free the name string if dynamically allcoated
void state_setName(State s, char* name);

// set a state as being a terminal state
void state_makeTerminal(State s);

// set a state as being a non-terminal state
void state_makeNonTerminal(State s);

// destroy a state and all transitions from it
// use this instead of free
void state_destroy(State s);

// add a transition from one state to another through the transition string
// you free the transition_string string if dynamically allocated
void state_addTransition(State s, char* transition_string, State dest);

// remove a transition from one state to another through the transition string
void state_removeTransition(State s, char* transition_string, State dest);

void nfa_print(NFA nfa);

void state_print(State s);

void transition_print(Transition t);

#endif