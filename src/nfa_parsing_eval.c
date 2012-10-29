#include "nfa_parsing_eval.h"
#include "strings.h"

int nfa_parsing_eval(NFA nfa, char* input) {
	return state_parsing_eval(nfa_initialState(nfa), input);
}

int state_parsing_eval(State s, char* input) {

	// if there are no transitions from here and it is a terminal state, return -1;
	// else if there are transitions:
		// go through all transitions
		// if all transitions fail and this is a terminal state, return -1;
		// else if all transitions fail, return 0;
		// else, return the length of the succssful transition;
	// else, return 0;

}

int transition_parsing_eval(Transition t, char* input) {

	// if the transition string is in the input string:
		// add the length of the transition string to the input string pointer
		// follow the transition to the next state
		// if the result of the next state is 0, return 0;
		// else if the result is -1, return the length of the transition;
		// else, return the length of the transition plus the result;
	// else, return 0;

}