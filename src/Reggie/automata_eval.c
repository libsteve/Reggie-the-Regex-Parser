#include <Reggie/automata_eval.h>

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
	if (s->isTerminal && input->closed(input)) {
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
		return state_eval(a, t->dst, input->fastforward(input, result));
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

int state_parsing_eval(automata a, state s, evalstream *input) {
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
		return state_parsing_eval(a, t->dst, input->fastforward(input, result));
	}
	return -1;
}
