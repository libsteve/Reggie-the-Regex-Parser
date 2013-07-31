#ifndef PDA_H
#define PDA_H

#include "list.h"

struct pda;
struct pda_state;
struct pda_transition;

// a function that a transition can use to determine whether or not the transition should be followed
// the function takes a PDA, the IDs of the source and destination states, 
// the input string, and any arbitrary data to be utilized by the function implementation.
// the function should return the length of an accepting string,
// 0 if the empty string is accepted or if the function passes due to data on the PDA's stack,
// and -1 if the transition should not be accepted.
typedef int (*pda_transition_fn)(struct pda *pda, int src, int dest, char *input, void *data);

// a structure to represent a state within a PDA
typedef struct pda_state {
	unsigned int id;	// an identifier number for the state
	int isTerminal;		// a boolean flag to dentoe that the state is termianl
	list transitions;	// a list of transitions from this state
} *PDAState;

// a structure to represent a PDA transition
typedef struct pda_transition {
	unsigned int id;			// an identifier number for the trnasition
	unsigned int source;		// the source state ID
	unsigned int dest;			// the destination state ID
	pda_transition_fn function;	// the PDA transition function
} *PDATransition;

// a structure to represent a push-down automata
typedef struct pda {
	list states;		// a list of states in the PDA
	list transitions;	// a list of transitions between the states
	list stack;			// a stack of data that can be used by transition functions
} *PDA;

// create a new PDA instance
PDA pda_create();

// destroy a PDA instance
void pda_destroy(PDA pda);

// add a state to the PDA
int pda_addState(PDA pda);

// returns the state struct for the PDA, or NULL if no state was found
PDAState pda_getState(PDA pda, int state);

// check if the given state is terminal
int pda_state_isTerminal(PDA pda, int state);

// make the given state terminal
void pda_state_makeTerminal(PDA pda, int state);

// make the given state non-terminal
void pda_state_makeNonTerminal(PDA pda, int state);

// returns the transition struct for the PDA, or NULL if no transition was found
PDATransition pda_getTransition(PDA pda, int transition);

// add a transition between the given states over the transition function
int pda_addTransition(PDA pda, int src, int dest, pda_transition_fn func);

// remove the given transition
void pda_removeTransition(PDA pda, int transition);

// get the stack for the PDA
list pda_stack(PDA pda);

#endif