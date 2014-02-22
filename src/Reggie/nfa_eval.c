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

int nfa_parsing_eval(NFA nfa, char* input) {
	struct stream s = stringstream(input);
	return automata_parsing_eval(&nfa->automata, s);
}
