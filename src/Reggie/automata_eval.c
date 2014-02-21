#include <Reggie/automata_eval.h>

//////
// total evaluations of automata

bool automata_eval(automata a, struct stream input) {
	FOREACH(it, a->states) {
		state s = VALUE(it);
		if (s->id == 0) {
			return state_eval(a, s, input);
		}
	}
	return false;
}

bool state_eval(automata a, state s, struct stream input) {
	if (s->isTerminal && input.closed(input)) {
		return true;
	}
	FOREACH(it, s->transitions) {
		transition t = VALUE(it);
		if (transition_eval(a, t, input)) {
			return true;
		}
	}
	return false;
}

bool transition_eval(automata a, transition t, struct stream input) {
	transition_result result = t->func(a, t, input);
	if (result.success) {
		return state_eval(a, t->dst, result.stream);
	}
	return false;
}

//////
// partial parsing evaluations of automata

#define failure -1

int automata_parsing_eval(automata a, struct stream input) {
	FOREACH(it, a->states) {
		state s = VALUE(it);
		if (s->id == 0) {
			return state_parsing_eval(a, s, input);
		}
	}
	return failure;
}

int state_parsing_eval(automata a, state s, struct stream input) {
	int longest = failure;
	if (s->isTerminal) {
		longest = 0;
	}
	FOREACH(it, s->transitions) {
		transition t = VALUE(it);
		int result = transition_parsing_eval(a, t, input);
		if (result > longest) {
			longest = result;
		}
	}
	return longest;
}

int transition_parsing_eval(automata a, transition t, struct stream input) {
	transition_result result = t->func(a, t, input);
	if (result.success) {
		int length = state_parsing_eval(a, t->dst, result.stream);
		if (length != failure) {
			length += result.length;
		}
		return length;
	}
	return failure;
}
