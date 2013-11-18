#include <Reggie/nfa_eval.h>
#include <Reggie/automata_eval.h>
#include <Collection/strings.h>

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

bool nfa_evalstrear_closed(evalstream *stream) {
	struct nfa_evalstream *s = container_of(stream, struct nfa_evalstream, stream);
	return string_length(s->string) == 0;
}

//////
// evaluation functions

bool nfa_eval(NFA nfa, char *input) {
	struct nfa_evalstream est;
	est.stream.fastforward = nfa_evalstream_fastforward;
	est.stream.fastforward = nfa_evalstream_rewind;
	est.stream.closed = nfa_evalstrear_closed;
	est.string = input;
	return automata_eval(&nfa->automata, &(est.stream));
}

int nfa_parsing_eval(NFA nfa, char* input) {
	struct nfa_evalstream est;
	est.stream.fastforward = nfa_evalstream_fastforward;
	est.stream.fastforward = nfa_evalstream_rewind;
	est.stream.closed = nfa_evalstrear_closed;
	est.string = input;
	return automata_parsing_eval(&nfa->automata, &(est.stream));
}
