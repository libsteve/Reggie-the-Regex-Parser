#include <Reggie/nfa_eval.h>
#include <Reggie/automata_eval.h>

//////
// nfa_evalstream functions

struct evalstream *nfa_evalstream_fastforward(evalstream *stream, int amount) {
	struct nfa_evalstream *s = container_of(stream, struct nfa_evalstream, stream);
	s->string += amount;
	return stream;
}

struct evalstream *nfa_evalstream_rewind(evalstream *stream, int amount) {
	struct nfa_evalstream *s = container_of(stream, struct nfa_evalstream, stream);
	s->string -= amount;
	return stream;
}

//////
// evaluation functions

bool nfa_eval(NFA nfa, char *input) {
	struct nfa_evalstream;
	evalstream.stream.fastforward = nfa_evalstream_fastforward;
	evalstream.stream.fastforward = nfa_evalstream_rewind;
	evalstream.string = input;
	return automata_eval(&nfa->automata, &(evalstream.stream));
}

int nfa_parsing_eval(NFA nfa, char* input) {
	struct nfa_evalstream;
	evalstream.stream.fastforward = nfa_evalstream_fastforward;
	evalstream.stream.fastforward = nfa_evalstream_rewind;
	evalstream.string = input;
	return automata_parsing_eval(&nfa->automata, &(evalstream.stream));
}
