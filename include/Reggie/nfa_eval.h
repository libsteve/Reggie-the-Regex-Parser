#ifndef REGGIE_NFA_EVAL_H
#define REGGIE_NFA_EVAL_H

#include <Reggie/nfa.h>

// evaluate the nfa with the given input string
// returns true if the input string matches the nfa, false otherwise
bool nfa_eval(NFA nfa, char *input);

// evaluate the nfa with the given input string
// returns -1 if failure or the length of the first successful substring match
int nfa_parsing_eval(NFA nfa, char *input);

struct nfa_tokenizing_result {
	bool success;
	struct {
		char *rest;
		struct lex_token token;
	};
};

// evaluate the nfa with the given input string to get the proper resulting token
struct nfa_tokenizing_result nfa_tokenizing_eval(NFA nfa, char *input);

#endif
