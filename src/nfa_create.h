#ifndef NFA_CREATE_H
#define NFA_CREATE_H

#include "list.h"
#include "nfa.h"

// a structure to represent a mold for a transition
typedef struct transition_mold {
	unsigned int source;	// the id of the source state
	char *string;			// the trnasition string
	unsigned int dest;		// the id of the destination state
} *TransitionMold;

// a structure to represent a mold for a state
typedef struct state_mold {
	unsigned int id;	// the id of the state
	int isTerminal;		// a flag to indicate whether or not this state is terminal
} *StateMold;

// a structure to represent a mold for an nfa
typedef struct nfa_mold {
	list states;		// a list of all the state molds in the nfa
	list transitions;	// a list of all transition molds in the nfa
	unsigned int state_count;	// the amount of states within the nfa
} *NFAMold;

// create a new nfa mold instance
// created with a default initial state with ID 0
NFAMold nfa_mold_create();

// destroy an nfa mold
void nfa_mold_destroy(NFAMold mold);

// add a state to the given nfa mold
// returns the id of the created state
unsigned int nfa_mold_createState(NFAMold mold);

// return 1 if the given state is terminal in the mold
void nfa_mold_isStateTerminal(NFAMold mold, unsigned int id);

// make the given state terminal in the mold
void nfa_mold_makeStateTerminal(NFAMold mold, unsigned int id);

// make the given state non-terminal in the mold
void nfa_mold_makeStateNonTerminal(NFAMold mold, unsigned int id);

// add a transition from the s1 state to the s2 state over the given transition string
void nfa_mold_addTransition(NFAMold mold, unsigned int s1, char *string, unsigned int s2);

// remove a transition from the s1 state to the s2 state over the given transition string
void nfa_mold_removeTransition(NFAMold mold, unsigned int s1, char *string, unsigned int s2);

// compile the mold into an NFA
NFA nfa_mold_compile(NFAMold mold);

// a convenience function for creating an NFA from a mold
#define nfa_createFromMold(mold) (nfa_mold_compile((mold)))

#endif