#include <stdlib.h>
#include <Reggie/nfa.h>
#include <Collection/strings.h>

NFA nfa_create() {
	NFA *nfa = calloc(1, sizeof(struct nfa));
	return nfa_initialize(nfa, nfa_uninitialize);
}

void nfa_destroy(NFA nfa) {
	nfa_uninitialize(nfa);
	free(nfa);
}

void nfa_state_destroy(NFAState s) {
	nfa_state_uninitialize(s);
	free(s);
}

void nfa_transition_destroy(NFATransition t) {
	nfa_transition_uninitialize(t);
	free(t);
}

//////
// Initialization Functions

NFA nfa_initialize(NFA nfa, automata_uninitialize uninitialize) {
	nfa = automata_initialize(&nfa->automata, uninitialize);
	NFAState s = calloc(1, sizeof(struct nfa_state));
	nfa_state_initialize(s, false, nfa_destroy);
	automata_addState(&nfa->automata, &s->state);
	return nfa;
}

void nfa_uninitialize(automata a) {
	automata_uninitialize(a);
}

NFAState nfa_state_initialize(NFAState s, bool isTerminal, state_destroy destroy) {
	state_initialize(&s->state, isTerminal, destroy);
	return s;
}

void nfa_state_uninitialize(state s) {
	state_uninitialize(s);
}


NFATransition nfa_transition_initialize(NFATransition t, state src, state dest, char *transition_string, transition_destroy destroy) {
	transition_initialize(&t->transition, src, dest, nfa_transition_func, destroy);
	t->transition_string = string_copy(transition_string);
	return t;
}

void nfa_transition_uninitialize(transition t) {
	NFATransition nfat = container_of(t, struct nfa_transition, transition);
	free(nfat->transition_string);
	transition_uninitialize(t);
}

int nfa_transition_func(const struct automata *a, const struct transition *t, const evalstream *input) {
	NFATransition nfat = container_of(t, struct nfa_transition, transition);
	struct nfa_evalstream *stream = container_of(input, struct nfa_evalstream, stream);
	if (string_substring(stream->string, nfat->transition_string)) {
		return string_length(nfat->transition_string);
	}
	return -1;
}


//////
// NFA Interaction Functions

state_id nfa_initialState(NFA nfa) {
	return 0;
}

state_id nfa_addState(NFA nfa) {
	NFAState s = calloc(1, sizeof(struct nfa_state));
	return nfa_state_initialize(s, false, nfa_state_destroy);
}

void nfa_removeState(NFA nfa, state_id sid) {
	state s = automata_findState(&nfa->automata, sid);
	if (s) {
		FOREACH(it, s->transitions) {
			transition t = VALUE(it);
			nfa_removeTransition(nfa, t->id);
		}
		s->uninitialize(s);
		free(container_of(s, struct nfa_state, state));
	}
}

bool nfa_state_isTerminal(NFA nfa, state_id sid) {
	state s = automata_findState(&nfa->automata, sid);
	if (s) {
		return s->isTerminal;
	}
	return false;
}

void nfa_state_setIsTerminal(NFA nfa, state_id sid, bool isTerminal) {
	state s = automata_findState(&nfa->automata, sid);
	if (s) {
		s->isTerminal = isTerminal;
	}
}

transition_id nfa_addTransition(NFA nfa, state_id sid1, state_id sid2, char *transition_string) {
	state s1 = automata_findState(&nfa->automata, sid1);
	state s2 = automata_findState(&nfa->automata, sid2);
	if (s1 && s2) {
		NFATransition t = calloc(1, sizeof(struct nfa_transition));
		nfa_transition_initialize(t, s1, s2, transition_string, nfa_transition_destroy);
		automata_addTransition(&nfa->automata, &t->transition);
		return t->transition.id;
	}
	return -1;
}

void nfa_removeTransition(NFA nfa, transition_id tid) {
	automata_removeTransition(&nfa->automata, tid);
}



#include <stdio.h>

void nfa_print(NFA a) {
	printf("Intital: %d\n", 0);
	state_print(list_getFrom(a->automata.states, 0));
}

void state_print(NFAState s) {
	char* terminal = s->state.isTerminal ? "!" : "";
	printf("%d:\t%s\n", s->state.id, terminal);
	list_foreach(s->transitions, (foreach_func)&transition_print);
}

void transition_print(NFATransition t) {
	printf("\t%d --\"%s\"--> %d\n", t->transition.src->state.id, t->transition_string, t->transition.dst->state.id);
}