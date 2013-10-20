#ifndef NFA_PARSING_EVAL_H
#define NFA_PARSING_EVAL_H

#include "nfa.h"

// evaluate the nfa with the given input string
// returns the length of some passing string
int nfa_parsing_eval(NFA nfa, char* input);

// evaluate the state with the given input string
// recursively evaluates through transitions
// returns the length of some passing string
// returns -1 if instant pass without length
// returns 0 if it fails the state
int state_parsing_eval(State s, char* input);

// follow and evaluate the transition with the given input string
// remove the transition string from the input
// evaluates destination state
// returns the length of some passing string
int transition_parsing_eval(Transition t, char* input);

#endif