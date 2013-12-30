#include <Reggie/pda_copy.h>
#include <Reggie/automata_copy.h>
#include <Collection/list_sort.h>

automata pda_copy_create() {
	PDA p = pda_create();
	return &p->automata;
}

state_id pda_copy_initialState(automata a) {
	PDA p = container_of(a, struct pda, automata);
	return pda_initialState(p);
}

state_id pda_copy_copyState(automata destination, state s) {
	PDA pda_dst = container_of(destination, struct pda, automata);
	state_id new = pda_addState(pda_dst);
	if (s->isTerminal) {
		pda_state_makeTerminal(pda_dst, new);
	} else {
		pda_state_makeNonTerminal(pda_dst, new);
	}
	return new;
}

transition_id pda_copy_copyTransition(automata dest, transition t, state_id newSrc, state_id newDst) {
	PDA pda = container_of(dest, struct pda, automata);
	PDATransition pda_t = container_of(t, struct pda_transition, transition);
    transition_id new = 0;
    switch (pda_t->type) {
    case PDA_TRANSITION_TYPE_STRING:
        new = pda_addStringTransition(pda, newSrc, newDst, t->string, t->pop_token, t->push_token)
        break;
    
    case PDA_TRANSITION_TYPE_NFA:
        new = pda_addNFATransition(pda, newSrc, newDst, t->nfa, t->pop_token, t->push_token);
        break;
    
    case PDA_TRANSITION_TYPE_PDA:
        new = pda_addPDATransition(pda, newSrc, newDst, t->pda, t->pop_token, t->push_token);
        break;
    
    case PDA_TRANSITION_TYPE_APPLICATION:
        new = pda_addApplicationTransition(pda, newSrc, newDst, t->apply, t->revoke);
        break;
    }
	return new;
}

// create a copy of an pda from a given pda
PDA pda_copy(PDA pda) {
	automata a = automata_copy(&pda->automata, (automata_copy_creation){pda_copy_create, pda_copy_initialState, pda_copy_copyState, pda_copy_copyTransition});
	return container_of(a, struct pda, automata);
}

