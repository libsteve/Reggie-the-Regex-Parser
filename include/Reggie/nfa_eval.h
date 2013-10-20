#ifndef NFA_EVAL_H
#define NFA_EVAL_H

#include "nfa.h"

// evaluate the nfa with the given input string
// returns 1 if the input passes the nfa, 0 otherwise
int nfa_eval(NFA nfa, char* input);

// evaluate the state with the given input string
// recursively evaluates through transitions
// returns 1 if the input passes the state, 0 otherwise
int state_eval(State s, char* input);

// follow and evaluate the transition with the given input string
// remove the transition string from the input
// evaluates destination state
// returns 1 if the input passes the transition, 0 otherwise
int transition_eval(Transition t, char* input);

#endif