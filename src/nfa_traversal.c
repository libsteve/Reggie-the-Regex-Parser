#include "nfa_traversal.h"
#include "strings.h"
#include "list.h"

int nfa_eval(NFA nfa, char* input) {
	return state_eval(nfa->initialState, input);
}

// a state data structure for use with state_eval_foreachWithState_func()
struct state_eval_foreachWithState_state {
	char*  input;
	list transitions;
};

// this is the foreach function needed to iterate
// through each transition in the state and end with
// a list of all transitions that apply to the input string
void state_eval_foreachWithState_func(void* state, void* value) {
	Transition t = (Transition)value;
	struct state_eval_foreachWithState_state* s = (struct state_eval_foreachWithState_state state*)state;

	if (string_substring(s->input, t->transition_string))
		list_push(s->transitions, t);
}

// a state data structure for use with state_eval_find_func()
struct state_eval_find_func_state {
	char* input;
	int result_of_transition;
};

// this is the find function used to traverse through each transition
// until a successful path is found. if a successful transition is found,
// the result_of_transition in the state is set to the result of 
int state_eval_find_func(void* state, void* value) {
	struct state_eval_find_func_state* s = (struct state_eval_find_func_state*)state;
	Transition t = (Transition)value;

	// evaluate the transition. if the transition passes, we have "found" our transition
	s->result_of_transition = transition_eval(t, s->input);
	if (s->result_of_transition)
		return 1;
	return 0;
}

int state_eval(State s, char* input) {
	// create a list of acceptable transitions
	list transition_list = list_create();

	// find all of the acceptable transitions from this state
	struct state_eval_foreachWithState_state foreachState = (struct state_eval_foreachWithState_state){input, transition_list};
	list_foreachWithState(s->transitions, &state_eval_foreachWithState_func, &foreachState);

	// try all the transitions until the strign matches or there are no more transitions
	struct state_eval_find_func_state findState = (struct state_eval_find_func_state){input, 0};
	list_find(transition_list, &state_eval_find_func, &findState);

	// destroy the transitions list
	list_destroy(transition_list);

	return findState.result_of_transition;
}

int transition_eval(Transition t, char* input) {
	if (string_substring(input, t->transition_string)) {
		input += string_length(t->transition_string);
		return state_eval(t->dest, input);
	}
	return 0;
}