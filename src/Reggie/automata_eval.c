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
	struct transition_result result = t->apply(a, t, input);
	if (result.success) {
		bool success = state_eval(a, t->dst, result.stream);
		if (t->revoke) t->revoke(a, t);
		if (result.lexed) vector_destroy(result.lexed);
		return success;
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
	int longest = s->isTerminal ? 0 : failure;
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
	struct transition_result result = t->apply(a, t, input);
	if (result.success) {
		int length = state_parsing_eval(a, t->dst, result.stream);
		if (length != failure) {
			length += result.length;
		}
		if (t->revoke) t->revoke(a, t);
		if (result.lexed) vector_destroy(result.lexed);
		return length;
	}
	return failure;
}

//////
// tokenizing evaluations of automata

struct tokenizing_result automata_tokenizing_eval(automata a, struct stream input) {
	FOREACH(it, a->states) {
		state s = VALUE(it);
		if (s->id == 0) {
			vector lexed = vector_create(input.peek_size(input));
			struct tokenizing_result result = state_tokenizing_eval(a, s, input, lexed);
			vector_destroy(lexed);
			return result;
		}
	}
	return (struct tokenizing_result){.success = false};
}

static void _destroy_result(struct tokenizing_result tr) {
	if (tr.token) {
		if (tr.destroy) tr.destroy(tr.token);
		else free(tr.token);
	}
}

struct tokenizing_result state_tokenizing_eval(automata a, state s, struct stream input, vector lexed) {
	struct tokenizing_result longest;
	if (s->isTerminal) {
		longest = (struct tokenizing_result){.success = true, .length = 0, .trailing = input, .token = NULL, .destroy = NULL};
	} else {
		longest = (struct tokenizing_result){.success = false};
	}
	FOREACH(it, s->transitions) {
		transition t = VALUE(it);
		struct tokenizing_result result = transition_tokenizing_eval(a, t, input, lexed);
		if (result.success) {
			if (!longest.success || result.length > longest.length) {
				_destroy_result(longest);
				longest = result;
			} else {
				_destroy_result(result);
			}
		}
	}
	return longest;
}

struct tokenizing_result transition_tokenizing_eval(automata a, transition t, struct stream input, const vector lexed) {
	struct transition_result result = t->apply(a, t, input);
	if (result.success) {
		vector _lexed = result.lexed ? vector_appendCreate(lexed, result.lexed) : lexed;
		struct tokenizing_result solution = state_tokenizing_eval(a, t->dst, result.stream, _lexed);
		if (solution.success) {
			solution.length += result.length;
		}
		if (t->revoke) t->revoke(a, t);
		if (result.lexed) vector_destroy(result.lexed);
		if (_lexed != lexed) vector_destroy(_lexed);
		return solution;
	}
	return (struct tokenizing_result){.success = false};
}
