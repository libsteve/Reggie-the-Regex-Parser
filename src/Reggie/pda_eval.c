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
        data->popped_tokens = data->apply(pda);
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
        data->revoke(pda, data->popped_tokens);
        if (data->popped_tokens != NULL) {
            list_destroy(data->popped_tokens);
            data->popped_tokens = NULL;
        }
        break;
    }

    return true;
}

bool pda_evalstream_closed(evalstream *stream) {
    struct pda_evalstream *stream = container_of(s, struct pda_evalstream, evalstream);
    return string_length(stream->string) == 0;
}

//////
// evaluation functions

// evaluate the pda with the given input string
// returns -1 if failure or the length of the first successful substring match
int __pda_eval(PDA pda, char* input, void **data_result, void (**result_destroy)(void *data)) {
    struct pda_evalstream est;
    est.stream.fastforward = pda_evalstream_fastforward;
    est.stream.rewind = pda_evalstream_rewind;
    est.stream.closed = pda_evalstrear_closed;
    est.string = input;

    pda->stack = list_create();
    int length_result = automata_parsing_eval(&pda->automata, &(est.stream));
    if (data_result) {
        PDAToken t = pda_popToken(pda);
        if (t) {
            switch (t->type) {
            case PDA_TOKEN_DATA:
                *data_result = t->data;
                if (result_destroy) {
                    *result_destroy = t->data_destroy;
                }
                t->data = NULL;
                break;
            case PDA_TOKEN_STRING:
                *data_result = t->string;
                if (result_destroy) {
                    *result_destroy = free;
                }
                t->string = NULL;
                break;
            }
            free(t);
        }
    }
    FOREACH(it, pda->stack) {
        PDAToken t = VALUE(it);
        t->destroy(t);
    }
    list_destroy(pda->stack);
    pda->stack = NULL;

    return length_result;
}

// evaluate the pda with the given input string
// returns true if the input string matches the pda, false otherwise
bool pda_eval(PDA pda, char *input) {
    int length = __pda_eval(pda, input, NULL, NULL);
    if (length == -1) {
        return false;
    }
    return true;
}

// evaluate the pda with the given input string
// returns -1 if failure or the length of the first successful substring match
int pda_parsing_eval(PDA pda, char *input) {
    return __pda_eval(pda, input, NULL, NULL);
}

// evaluate the pda with the given input string
// returns a structure with:
//      a success of false if failure or ture otherwise
//      a length of -1 if failure or the first successful substring match
//      a data of NULL if failure or the successful data result
//      a data_destroy function to properly destroy the data result or NULL
pda_eval_result pda_data_eval(PDA pda, char *input) {
    pda_eval_result result;
    result.success = true;
    result.data = NULL;
    result.data_destroy = NULL;
    result.length = __pda_eval(pda, input, &result.data, &result.data_destroy);
    if (length == -1) {
        result.success = false;
    }
    return result;
}

