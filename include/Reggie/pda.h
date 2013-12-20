#ifndef REGGIE_PDA_H
#define REGGIE_PDA_H

#include <Collection/list.h>
#include <Reggie/automata.h>
#include <Reggie/nfa.h>

struct pda;
struct pda_state;
struct pda_transition;
struct pda_evalstream;

typedef void (*pda_transition_apply_func)(const struct pda *pda, const struct pda_transition *t, const pda_evalstream *input, );
typedef void (*pda_transition_revoke_func)(const struct pda *pda, const struct pda_transition *t, const pda_evalstream *input, );

// a structure to represent a state within a PDA
typedef struct pda_state {
	struct state state;
} *PDAState;

// a structure to represent a PDA transition
typedef struct pda_transition {
	struct state state;
    enum pda_transition_type {
        PDA_TRANSITION_TYPE_STRING,
        PDA_TRANSITION_TYPE_NFA,
        PDA_TRANSITION_TYPE_PDA
    } type;
    union {
        char *string;   // an allocated, null-terminated transition string
        NFA nfa;        // a copied NFA
        PDA pda;        // a copied PDA
    };
	pda_transition_apply_func apply;    // apply this when perfroming the transition
    pda_transition_revoke_func revoke;  // undo the application of this transiton
} *PDATransition;

// a structure to represent a push-down automata
typedef struct pda {
	struct automata automata;
	list stack;			// a stack of data that can be used by transition functions
} *PDA;

// create a new PDA instance
PDA pda_create();

// destroy a PDA instance
void pda_destroy(PDA pda);


//////
// Initialization Functions

PDA pda_initialize(PDA pda, automata_destroy destroy);
void pda_uninitialize(automata a);

PDAState pda_state_initialize(PDAState s, bool isTerminal, state_destroy destroy);
void pda_state_uninitialize(state s);

PDATransition nfa_transition_initialize(PDATransition t, state src, state dest, char *transition_string, transition_destroy destroy);
void pda_transition_uninitialize(transition t);

int pda_transition_func(const struct automata *a, const struct transition *t, const evalstream *input); 

// evalstream instance for pda_transition_func
struct pda_evalstream {
	struct evalstream stream;
	char *string;
};



//////
// PDA Interaction Functions

// add a state to the PDA's list
// returns -1 if failure
state_id pda_addState(PDA pda);
void pda_removeState(PDA pda, state_id sid);

bool pda_state_isTerminal(PDA pda, state_id sid);
void pda_state_setIsTerminal(PDA pda, state_id sid, bool isTerminal);
#define pda_state_makeTerminal(pda, sid) 	pda_state_setIsTerminal(pda, sid, true)
#define pda_state_makeNonTerminal(pda, sid) pda_state_setIsTerminal(pda, sid, false)

transition_id pda_addStringTransition(PDA pda, state_id sid1, state_id sid2, char *transition_string);
transition_id pda_addNFATransition(PDA pda, state_id sid1, state_id sid2, NFA nfa);
transition_id pda_addPDATransition(PDA pda, state_id sid1, state_id sid2, PDA pda);
void pda_removeTransition(PDA pda, transition_id tid);

list pda_stack(PDA pda);

#endif

