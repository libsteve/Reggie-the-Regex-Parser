#ifndef NFA_H
#define NFA_H

#include <Reggie/automata.h>

// a transition for an nfa
typedef struct nfa_transition {
	struct transition transition;
	char *transition_string; // a string to transition through when encountered
							 // this must be an allocated string
} *NFATransition;

// a state in an nfa
typedef struct nfa_state {
	struct state state;
} *NFAState;

// a non-deterministic finite automata
typedef struct nfa {
	struct automata automata;
} *NFA;

// create a new nfa object
// all NFAs are created with an initial state
NFA nfa_create();

// destroy an nfa object and all states within
// use this instead of free
void nfa_destroy(NFA nfa);


//////
// Initialization Functions

NFA nfa_initialize(NFA nfa, automata_destroy destroy);
void nfa_uninitialize(automata a);

NFAState nfa_state_initialize(NFAState s, bool isTerminal, state_destroy destroy);
void nfa_state_uninitialize(state s);

NFATransition nfa_transition_initialize(NFATransition t, state src, state dest, char *transition_string, transition_destroy destroy);
void nfa_transition_uninitialize(transition t);

int nfa_transition_func(const struct automata *a, const struct transition *t, const evalstream *input); 

// evalstream instance for nfa_transition_func
struct nfa_evalstream {
	struct evalstream stream;
	char *string;
};


//////
// NFA Interaction Functions

// get the initial state of the NFA
state_id nfa_initialState(NFA nfa);

// add a state to the NFA's state list
// returns -1 if failure
state_id nfa_addState(NFA nfa);
#define nfa_makeState(nfa) nfa_addState(nfa)
void nfa_removeState(NFA nfa, state_id sid);

bool nfa_state_isTerminal(NFA nfa, state_id sid);
void nfa_state_setIsTerminal(NFA nfa, state_id sid, bool isTerminal);
#define nfa_state_makeTerminal(nfa, sid) 	nfa_state_setIsTerminal(nfa, sid, true)
#define nfa_state_makeNonTerminal(nfa, sid) nfa_state_setIsTerminal(nfa, sid, false)

// returns -1 if failure
transition_id nfa_addTransition(NFA nfa, state_id sid1, state_id sid2, char *transition_string);
void nfa_removeTransition(NFA nfa, transition_id tid);


void nfa_print(NFA nfa);

void state_print(NFAState s);

void transition_print(NFATransition t);

#endif
