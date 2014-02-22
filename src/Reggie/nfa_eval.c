#include <Reggie/nfa_eval.h>
#include <Reggie/automata_eval.h>
#include <Collection/strings.h>
#include <Collection/stream.h>

//////
// evaluation functions

bool nfa_eval(NFA nfa, char *input) {
	struct stream s = stringstream(input);
	return automata_eval(&nfa->automata, s);
}

int nfa_parsing_eval(NFA nfa, char *input) {
	struct stream s = stringstream(input);
	return automata_parsing_eval(&nfa->automata, s);
}

struct nfa_tokenizing_result nfa_tokenizing_eval(NFA nfa, char *input) {
	struct stream s = stringstream(input);
	struct tokenizing_result result = automata_tokenizing_eval(&nfa->automata, s);
	if (result.success) {
		return (struct nfa_tokenizing_result){.success = true, .rest = input + result.length, .token = result.token};
	} else {
		return (struct nfa_tokenizing_result){.success = false};
	}
}
