#include "Reggie/nfa_parsing_eval.h"
#include "Collection/strings.h"

int nfa_parsing_eval(NFA nfa, char* input) {
	return state_parsing_eval(nfa_initialState(nfa), input);
}

// a state data structure for use with state_parsing_eval_find_func()
struct state_parsing_eval_find_func_state {
	char* input;
	int result_of_transition;
};

// this is the find function used to traverse through each transition
// until a successful path is found. if a successful transition is found,
// the result_of_transition in the state is set to the result of 
int state_parsing_eval_find_func(void* state, void* value) {
	struct state_parsing_eval_find_func_state* s = (struct state_parsing_eval_find_func_state*)state;
	Transition t = (Transition)value;

	// evaluate the transition. if the transition passes, we have "found" our transition
	s->result_of_transition = transition_parsing_eval(t, s->input);
	if (s->result_of_transition)
		return 1;
	return 0;
}

int state_parsing_eval(State s, char* input) {
	// if there are no transitions from here and it is a terminal state, return -1;
	// else if there are transitions:
		// go through all transitions
		// if all transitions fail and this is a terminal state, return -1;
		// else if all transitions fail, return 0;
		// else, return the length of the succssful transition;
	// else, return 0;

	if (s->isTerminalState && list_len(s->transitions) == 0) 
		return -1;
	else if (list_len(s->transitions) > 0) {

		// try all the transitions until the string matches or there are no more transitions
		struct state_parsing_eval_find_func_state findState = (struct state_parsing_eval_find_func_state){input, 0};
		list_find(s->transitions, &state_parsing_eval_find_func, &findState);

		// if the string never matched but this is a terminal state, it is a pass, return -1;
		if (s->isTerminalState && findState.result_of_transition == 0) return -1;
		else return findState.result_of_transition;

	} else return 0;
}

int transition_parsing_eval(Transition t, char* input) {
	// if the transition string is in the input string:
		// add the length of the transition string to the input string pointer
		// follow the transition to the next state
		// if the result of the next state is 0, return 0;
		// else if the result is -1, return the length of the transition;
		// else, return the length of the transition plus the result;
	// else, return 0;

	if (string_substring(input, t->transition_string)) {
		int length = string_length(t->transition_string);
		input += length;
		int result = state_parsing_eval(t->dest, input);
		if (result == -1) return length;
		else if (result > 0) return result + length;
	}
	return 0;
}