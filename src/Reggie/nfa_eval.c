#include <Reggie/nfa_eval.h>
#include <Reggie/automata_eval.h>

bool nfa_eval(NFA nfa, char *input) {
	return automata_eval(&nfa->automata, input);
}

int nfa_parsing_eval(NFA nfa, char* input) {
	return automata_parsing_eval(&nfa->automata, input);
}
