#include <Reggie/automata.h>
#include <Collection/strings.h>

//////
// total evaluations of automata

bool automata_eval(automata a, evalstream *input) {
	FOREACH(it, a->states) {
		state s = VALUE(it);
		if (s->id == 0) {
			return state_eval(a, s, input);
		}
	}
	return false;
}

bool state_eval(automata a, state s, evalstream *input) {
	if (s->isTerminal && string_length(input) == 0) {
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

bool transition_eval(automata a, transition t, evalstream *input) {
	int result = t->func(a, t, input);
	if (result != -1) {
		return state_eval(a, s, input->fastforward(input, result));
	}
	return false;
}

//////
// partial parsing evaluations of automata

int automata_parsing_eval(automata a, evalstream *input) {
	FOREACH(it, a->states) {
		state s = VALUE(it);
		if (s->id == 0) {
			return state_parsing_eval(a, s, input);
		}
	}
	return -1;
}

int state_parsing_eval(automata a, state a, evalstream *input) {
	if (s->isTerminal) {
		return 0;
	}
	FOREACH(it, s->transitions) {
		transition t = VALUE(it);
		int result = transition_parsing_eval(a, t, input);
		if (result != -1) {
			return result;
		}
	}
	return -1;
}

int transition_parsing_eval(automata a, transition t, evalstream *input) {
	int result = t->func(a, t, input);
	if (result != -1) {
		return state_parsing_eval(a, s, input->fastforward(input, result));
	}
	return -1;
}
