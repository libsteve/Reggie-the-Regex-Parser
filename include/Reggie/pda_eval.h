#ifndef PDA_EVAL_H
#define PDA_EVAL_H

#include "pda.h"

// evaluate the pda with the given input string, using the given data for the transitions
// returns the length of some passing string
int pda_eval(PDA pda, char* input, void *data);

// evaluate the state with the given input string, using the given data for the transitions
// recursively evaluates through transitions
// returns the length of some passing string
// returns -1 if instant pass without length
// returns 0 if it fails the state
int pda_state_eval(PDA pda, PDAState s, char* input, void *data);

// follow and evaluate the transition with the given input string, using the given data for the transitions
// remove the transition string from the input
// evaluates destination state
// returns the length of some passing string
int pda_transition_eval(PDA pda, PDATransition t, char* input, void *data);

#endif