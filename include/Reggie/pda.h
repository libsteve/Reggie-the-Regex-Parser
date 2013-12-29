#ifndef REGGIE_PDA_H
#define REGGIE_PDA_H

#include <Collection/list.h>
#include <Reggie/automata.h>
#include <Reggie/nfa.h>

struct pda;
struct pda_state;
struct pda_transition;
struct pda_evalstream;
struct pda_token;

typedef list (*pda_transition_apply_func)(const struct pda *pda);
typedef void (*pda_transition_revoke_func)(const struct pda *pda, list popped_tokens);

// a structure to represent a push-down automata
typedef struct pda {
	struct automata automata;
	list stack;         // a stack of tokens
} *PDA;

// a structure to represent a state within a PDA
typedef struct pda_state {
	struct state state;
} *PDAState;

// a structure to represent a PDA transition
typedef struct pda_transition {
	struct transition transition;
    enum pda_transition_type {
        PDA_TRANSITION_TYPE_STRING,
        PDA_TRANSITION_TYPE_NFA,
        PDA_TRANSITION_TYPE_PDA,
        PDA_TRANSITION_TYPE_APPLICATION
    } type;
    union {
        char *string;   // an allocated, null-terminated transition string
        NFA nfa;        // a owned NFA
        PDA pda;        // a owned PDA
        struct {
            pda_transition_apply_func apply;
            pda_transition_revoke_func revoke;
        };
    };
	unsigned int pop_token;
    unsigned int push_token;
} *PDATransition;

struct pda_evaldata {
    enum {
        PDA_EVALDATA_MATCH,
        PDA_EVALDATA_APPLICATION
    } type;
    union {
        struct {
            int length;
            unsigned int pop_token;
            unsigned int push_token;
            PDAToken popped_token;
            PDAToken pushed_token;
        };
        struct {
            list popped_stack;
            list pushed_stack;
            pda_transition_apply_func apply;
            pda_transition_revoke_func revoke;
        };
    };
}
#define pda_evaldata_pushed_stack(d) \
    ((d->type == PDA_EVALDATA_APPLICATION) ? d->pushed_stack : NULL)
#define pda_evaldata_popped_stack(d) \
    ((d->type == PDA_EVALDATA_APPLICATION) ? d->popped_stack : NULL)

#define pda_dont_pop    ((unsigned int)-1)
#define pda_dont_push   ((unsigned int)-1)
#define pda_stack_empty ((unsigned int)-2)

typedef struct pda_token {
    unsigned int id;
    enum {
        PDA_TOKEN_STRING,
        PDA_TOKEN_DATA
    } type;
    union {
        char *string;
        struct {
            void *data;
            void data_destroy(void *data);
        };
    };
    void destroy(struct pda_token *token);
} *PDAToken;

// evalstream instance for pda_transition_func
struct pda_evalstream {
	struct evalstream stream;
	char *string;
    char *next_token;
    char *(*token)(struct pda_evalstream *evalstream);
};


//////
// PDA Interaction Functions

// create a new PDA instance
PDA pda_create();

// destroy a PDA instance
void pda_destroy(PDA pda);

// add a state to the PDA's list
// returns -1 if failure
state_id pda_addState(PDA pda);
void pda_removeState(PDA pda, state_id sid);

bool pda_state_isTerminal(PDA pda, state_id sid);
void pda_state_setIsTerminal(PDA pda, state_id sid, bool isTerminal);
#define pda_state_makeTerminal(pda, sid) 	pda_state_setIsTerminal(pda, sid, true)
#define pda_state_makeNonTerminal(pda, sid) pda_state_setIsTerminal(pda, sid, false)

transition_id pda_addStringTransition(PDA pda, state_id sid1, state_id sid2, char *transition_string, unsigned int pop_token, unsgined int push_token);
transition_id pda_addNFATransition(PDA pda, state_id sid1, state_id sid2, NFA nfa, unsigned int pop_token, unsigned int push_token);
transition_id pda_addPDATransition(PDA pda, state_id sid1, state_id sid2, PDA pda, unsigned int pop_token, unsigned int push_token);
transition_id pda_addApplicationTransition(PDA pda, state_id sid1, state_id sid2, pda_transition_apply_func apply, pda_transition_revoke_func revoke);
void pda_removeTransition(PDA pda, transition_id tid);

//////
// stack functions

void pda_pushToken(PDA pda, PDAToken token);
void pda_pushToken_string(PDA pda, int id, char *string);
void pda_pushToken_data(PDA pda, int id, void *data, void (*data_destory_func)(void *data));

// return pda_stack_empty when the stack is empty
unsigned int pda_peekToken_id(PDA pda);

PDAToken pda_popToken(PDA pda);

unsigned int pda_token_id(PDAToken token);
char *pda_token_string(PDAToekn token);
void *pda_token_data(PDAToken token);

#endif

