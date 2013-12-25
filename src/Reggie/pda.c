#include <Reggie/pda.h>
#include <Reggie/nfa_eval.h>
#include <Reggie/pda_eval.h>
#include <Collection/strings.h>

//////
// private functions

void _pda_state_destroy(state s) {
    PDAState state = contianer_of(s, struct pda_state, state);
    state_uninitialize(s);
    free(state);
}

void _pda_transition_destroy(transition t) {
    PDATransition transition = container_of(t, struct pda_transition, transition);
    switch (transition->type) {
    case PDA_TRANSITION_TYPE_STRING:
        free(transition->string);
        break;

    case PDA_TRANSITION_TYPE_NFA:
        nfa_destroy(transition->nfa);
        break;

    case PDA_TRANSITION_TYPE_PDA:
        pda_destroy(transition->pda);
        break;

    case PDA_TRANSITION_TYPE_APPLICATION:
        break;
    }
    transition_uninitialize(t);
    free(transition);
}

void _pda_destroy(automata a) {
    PDA pda = container_of(a, struct pda, automata);
    FOREACH(it, pda->stack) {
        PDAToken t = VALUE(it);
        t->destroy(t);
    }
    list_destroy(pda->stack);
    automata_uninitialize(a);
    free(pda);
}

void _pda_token_destory(PDAToken token) {
    switch (token->type) {
    case PDA_TOKEN_STRING:
        free(token->string);
        break;

    case PDA_TOKEN_DATA:
        token->data_destroy(token->data);
        break;
    }
    free(token);
};

void _pda_evaldata_destroy(void *d) {
    struct pda_evaldata data = *(struct pda_evaldata *)d;
    if (data.popped_stack) {
        FOREACH(it, data.popped_stack) {
            PDAToken t = VALUE(it);
            _pda_token_destroy(t);
        }
        list_destroy(data.popped_stack);
    }
    if (data.pushed_stack) {
        list_destroy(data.pushed_stack);
        FOREACH(it, data.popped_stack) {
            PDAToken t = VALUE(it);
            _pda_token_destroy(t);
        }
    }
    free(d);
}

// returns -1 if failure, otherwise returns the length of a success
evaldata pda_transition_func(const struct automata *a, const struct transition *t, const evalstream *input) {
    PDA pda = container_of(a, struct pda, automata);
    PDATransition transition = container_of(t, struct pda_transition, transition);
    struct pda_evalstream *stream = container_of(input, struct pda_evalstream, stream);
    struct evaldata result_data = {calloc(1, sizeof(struct pda_evaldata)), _pda_evaldata_destroy};
    struct pda_evaldata *data = result_data.data;

    // TODO: implement transition execution
    if (transition->type != PDA_TRANSITION_TYPE_APPLICATION) {
        data.length = -1;
        switch (transition->type) {
        case PDA_TRANSITION_TYPE_STRING:
            if (string_substring(stream->string, nfat->transition_string)) {
                data.length = string_length(nfat->transition_string);
            }
            break;

        case PDA_TRANSITION_TYPE_NFA:
            data.length = nfa_parsing_eval(transition->nfa, stream->string);
            break;

        case PDA_TRANSITION_TYPE_PDA:
            data.length = nfa_parsing_eval(transition->pda, stream->string);
            break;
        }

        if (transition->pop_token == )
    } else {
        data.length = 0;
    }
    
    return result_data;
}

//////
// interaction functions

// create a new PDA instance
PDA pda_create() {
    PDA pda = calloc(1, sizeof(struct pda));
    automata a = automata_initialize(&pda->automata, _pda_destroy);
    if (a) {
        pda->stack = list_create();
        return pda;
    }
    return NULL;
}

// destroy a PDA instance
void pda_destroy(PDA pda) {
    pda->automata.destroy(&pda->automata);
}


// add a state to the PDA's list
// returns -1 if failure
state_id pda_addState(PDA pda) {
    PDAState state = calloc(1, sizeof(struct pda_state));
    state s = state_initialize(&state->state, false, _pda_state_destroy);
    return automata_addState(&pda->automata, s);
}

void pda_removeState(PDA pda, state_id sid) {
    automata_removeState(&pda->automata, sid);
}

// state functions

bool pda_state_isTerminal(PDA pda, state_id sid) {
    state s = automata_findState(&pda->automata, sid);
    if (s) {
        return s->isTerminal;
    }
    return false;
}

void pda_state_setIsTerminal(PDA pda, state_id sid, bool isTerminal) {
    state s = automata_findState(&pda->automata, sid);
    if (s) {
        s->isTerminal = isTerminal;
    }
}

// transition functions

transition_id pda_addStringTransition(PDA pda, state_id sid1, state_id sid2, char *transition_string, unsigned int pop_token, unsgined int push_token) {
    PDATransition transition = calloc(1, sizeof(struct pda_transition));
    transition t = transition_initialize(&transition->transition, sid1, sid2, pda_transition_func, _pda_transition_destroy);
    transition->type = PDA_TRANSITION_TYPE_STRING;
    transition->string = string_copy(transition_string);
    transition->pop_token = pop_token;
    transition->push_token = push_token;
    return automata_addTransition(&pda->automata, t);
}

transition_id pda_addNFATransition(PDA pda, state_id sid1, state_id sid2, NFA nfa, unsigned int pop_token, unsgined int push_token) {
    PDATransition transition = calloc(1, sizeof(struct pda_transition));
    transition t = transition_initialize(&transition->transition, sid1, sid2, pda_transition_func, _pda_transition_destroy);
    transition->type = PDA_TRANSITION_TYPE_NFA;
    transition->nfa = nfa;
    transition->pop_token = pop_token;
    transition->push_token = push_token;
    return automata_addTransition(&pda->automata, t);
}

transition_id pda_addPDATransition(PDA pda, state_id sid1, state_id sid2, PDA pda, unsigned int pop_token, unsgined int push_token) {
    PDATransition transition = calloc(1, sizeof(struct pda_transition));
    transition t = transition_initialize(&transition->transition, sid1, sid2, pda_transition_func, _pda_transition_destroy);
    transition->type = PDA_TRANSITION_TYPE_PDA;
    transition->pda = pda;
    transition->pop_token = pop_token;
    transition->push_token = push_token;
    return automata_addTransition(&pda->automata, t);
}

transition_id pda_addApplicationTransition(PDA pda, state_id sid1, state_id sid2, pda_transition_apply_func apply, pda_transition_revoke_func revoke) {
    PDATransition transition = calloc(1, sizeof(struct pda_transition));
    transition t = transition_initialize(&transition->transition, sid1, sid2, pda_transition_func, _pda_transition_destroy);
    transition->type = PDA_TRANSITION_TYPE_APPLICATION;
    transition->apply = apply;
    transition->revoke = revoke;
    transition->pop_token = pda_dont_pop;
    transition->push_token = pda_dont_push;
    return automata_addTransition(&pda->automata, t);
}

void pda_removeTransition(PDA pda, transition_id tid) {
    automata_removeTransition(&pda->automata, tid);
}

//////
// stack functions

void pda_pushToken(PDA pda, PDAToken token) {
    list_push(pda->stack, token);
}

void pda_pushToken_string(PDA pda, unsigned int id, char *string) {
    PDAToken t = calloc(1, sizeof(struct pda_token));
    t->id = id;
    t->type = PDA_TOKEN_STRING;
    t->string = string_copy(string);
    t->destroy = _pda_token_destroy;
    return t;
}

void pda_pushToken_data(PDA pda, unsigned int id, void *data, void (*data_destory_func)(void *data)) {
    PDAToken t = calloc(1, sizeof(struct pda_token));
    t->id = id;
    t->type = PDA_TOKEN_DATA;
    t->data = data;
    t->data_destroy = data_destroy_func;
    t->destroy = _pda_token_destroy;
    return t;
}


unsigned int pda_peekToken_id(PDA pda) {
    PDAToken t = list_peek(pda->stack);
    if (t) {
        return t->id;
    }
    return pda_stack_empty;
}


PDAToken pda_popToken(PDA pda) {
    return list_pop(pda->stack);
}


unsigned int pda_token_id(PDAToken token) {
    return token->id;
}

char *pda_token_string(PDAToekn token) {
    if (t->type == PDA_TOKEN_STRING) {
        return t->string;
    }
    return NULL;
}

void *pda_token_data(PDAToken token) {
    if (t->type == PDA_TOKEN_DATA) {
        return t->data;
    }
    return NULL;
}

