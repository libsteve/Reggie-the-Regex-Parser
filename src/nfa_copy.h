#ifndef NFA_COPY_H
#define NFA_COPY_H

#include "map.h"
#include "nfa.h"

struct transition_template;
struct state_template;
struct nfa_template;

// a structure to represent a transition template
struct transition_template {
	char *string;	// the transition string
	void *source;	// the pointer value of the cource states
	void *dest;		// the pointer value of the destination state
} *TransitionTemplate;

// a structure to represent a state template
typedef struct state_template {
	int isTerminal;		// a boolean flag determining if this is a terminal state
	char *name;			// the name of the state
} *StateTemplate;

// a structure to represent an nfa template
typedef struct nfa_template {
	map states;			// a map of state pointers to a list of state templates
	map transitions;	// a map of transition pointers to transition templates
	void *initialState;	// a pointer value of the initial state of the nfa
} *NFATemplate;

// create an nfa template from a given nfa
NFATemplate nfa_template_create(NFA nfa);

// destroy a template nfa instance
void nfa_template_destroy(NFATemplate t);

// create a state template from a given state
StateTemplate state_template_create(State s);

// destroy a state template nfa instance
void state_template_destroy(StateTemplate st);

// add a state, all it's transitions, and all the connected states to the template
void nfa_template_addStateRecursive(NFATemplate t, State s);

// create a transition template from a given transition
TransitionTemplate transition_template_create(Transition t);

// destroy a transition template instance
void transition_template_destroy(TransitionTemplate tt);

// add a transition and all the connected states to the template
void nfa_template_addTransitionRecursive(NFATemplate t, Transition tr);

// create an nfa from a given nfa template
NFA nfa_createFromTemplate(NFATemplate t);

// create a copy of an nfa from a given nfa
NFA nfa_copy(NFA nfa);

#endif