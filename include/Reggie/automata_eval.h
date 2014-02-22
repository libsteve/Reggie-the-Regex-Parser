#ifndef REGGOE_AUTOMATA_EVAL_H
#define REGGOE_AUTOMATA_EVAL_H

#include <Reggie/automata.h>
#include <Collection/vector.h>

//////
// total evaluations of automata

// returns true if the input string matches the automata, false otherwise
bool automata_eval(automata a, struct stream input);
bool state_eval(automata a, state s, struct stream input);
bool transition_eval(automata a, transition t, struct stream input);

//////
// partial parsing evaluations of automata

// returns -1 if failure or the length of the first successful substring match
int automata_parsing_eval(automata a, struct stream input);
int state_parsing_eval(automata a, state s, struct stream input);
int transition_parsing_eval(automata a, transition t, struct stream input);

//////
// tokenizing evaluations of automata

struct tokenizing_result {
	bool success;
	
	// data during a success
	struct {
		size_t length;
		struct stream trailing;
		struct lex_token token;
	};
};

struct tokenizing_result automata_tokenizing_eval(automata a, struct stream input);
struct tokenizing_result state_tokenizing_eval(automata a, state s, struct stream input, const vector lexed);
struct tokenizing_result transition_tokenizing_eval(automata a, transition t, struct stream input, const vector lexed);

#endif
