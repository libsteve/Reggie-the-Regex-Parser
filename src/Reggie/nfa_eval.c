#include <Reggie/nfa_eval.h>
#include <Reggie/automata_eval.h>
#include <Collection/strings.h>

//////
// nfa_evalstream functions

#define nfa_evalstream_amount(data_pointer) (*((int *)(data_pointer)))

void *nfa_evalstream_fastforward(evalstream *stream, evaldata fastforward_data) {
	struct nfa_evalstream *s = container_of(stream, struct nfa_evalstream, stream);
	s->string += nfa_evalstream_amount(fastforward_data.data);
	return fastforward_data;
}

bool nfa_evalstream_rewind(evalstream *stream, evaldata rewind_data) {
	struct nfa_evalstream *s = container_of(stream, struct nfa_evalstream, stream);
	s->string -= nfa_evalstream_amount(rewind_data.data);
	return true;
}

bool nfa_evalstrear_closed(evalstream *stream) {
	struct nfa_evalstream *s = container_of(stream, struct nfa_evalstream, stream);
	return string_length(s->string) == 0;
}

#undef nfa_evalstream_amount

//////
// evaluation functions

bool nfa_eval(NFA nfa, char *input) {
	struct nfa_evalstream est;
	est.stream.fastforward = nfa_evalstream_fastforward;
	est.stream.rewind = nfa_evalstream_rewind;
	est.stream.closed = nfa_evalstrear_closed;
	est.string = input;
	return automata_eval(&nfa->automata, &(est.stream));
}

int nfa_parsing_eval(NFA nfa, char* input) {
	struct nfa_evalstream est;
	est.stream.fastforward = nfa_evalstream_fastforward;
	est.stream.rewind = nfa_evalstream_rewind;
	est.stream.closed = nfa_evalstrear_closed;
	est.string = input;
	return automata_parsing_eval(&nfa->automata, &(est.stream));
}

