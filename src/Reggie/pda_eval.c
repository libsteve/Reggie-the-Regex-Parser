#include <Reggie/pda_eval.h>
#include <Collection/strings.h>

//////
// pda_evalstream functions

evaldata pda_evalstream_fastforward(evalstream *s, automata a, evaldata fastforward_data) {
    struct pda_evalstream *stream = container_of(s, struct pda_evalstream, evalstream);
    PDA pda = container_of(a, struct automata, automata);
    struct pda_evaldata *data = fastforward_data.data;

    switch (data->type) {
    case PDA_EVALDATA_MATCH:
        {
            unsigned int peeked_id = pda_peekToken_id(pda);
            if (data->pop_token != pda_dont_pop && data->pop_token == peeked_id) {
                if (peeked_id != pda_stack_empty) {
                    data->popped_token = pda_popToken(pda);
                }
            } 
            if ((data->pop_token != pda_dont_pop || data->pop_token == peeked_id) && data->push_token != pda_dont_push) {
                data->pushed_token = pda_pushToken_string(pda, data->push_token, string_copy_substring(stream->string, data->length);
            }
        }
        stream->string += data->length;
        break;

    case PDA_EVALDATA_APPLICATION:
        
        break;
    }

    return fastforward_data;
}

bool pda_evalstream_rewind(evalstream *stream, automata a, evaldata rewind_data) {
    struct pda_evalstream *stream = container_of(s, struct pda_evalstream, evalstream);
    PDA pda = container_of(a, struct automata, automata);
    struct pda_evaldata *data = rewind_data.data;

    switch (data->type) {
    case PDA_EVALDATA_MATCH:
        if (data->pushed_token != NULL) {
            pda_popToken(pda);
            data->pushed_token->destroy(data->pushed_token);
            data->pushed_token = NULL;
        }
        if (data->popped_token != NULL) {
            pda_pushToken(pda, data->popped_token);
            data->popped_token = NULL;
        }
        stream->string -= data->length;
        break;

    case PDA_EVALDATA_APPLICATION:
        
        break;
    }

}

bool pda_evalstream_closed(evalstream *stream) {
    struct pda_evalstream *stream = container_of(s, struct pda_evalstream, evalstream);



}

//////
// evaluation functions

// evaluate the pda with the given input string
// returns -1 if failure or the length of the first successful substring match
int pda_parsing_eval(PDA pda, char* input) {
    struct pda_evalstream est;
    est.stream.fastforward = pda_evalstream_fastforward;
    est.stream.rewind = pda_evalstream_rewind;
    est.stream.closed = pda_evalstrear_closed;
    est.string = input;
    return automata_parsing_eval(&pda->automata, &(est.stream));
}

// evaluate the pda with the given input string
// returns true if the input string matches the pda, false otherwise
bool pda_eval(PDA pda, char *input) {
    
}

