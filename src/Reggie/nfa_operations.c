#include "Reggie/nfa_operations.h"
#include "Collection/list.h"
#include "Reggie/nfa_create.h"
#include "Reggie/nfa_copy.h"

NFA nfa_CONCAT(NFA a, NFA b) {	
	NFAMold mold_a = nfa_decompile(a);
	NFAMold mold_b = nfa_decompile(b);

	FOREACH(it, mold_b->states) {
		StateMold sm = VALUE(it);
		sm->id += mold_a->state_count;
	}
	FOREACH(it, mold_b->transitions) {
		TransitionMold tm = VALUE(it);
		tm->source += mold_a->state_count;
		tm->dest += mold_a->state_count;
	}

	list states_to_transition = list_create();
	unsigned int mold_b_initialState = mold_a->state_count;
	FOREACH(it, mold_a->states) {
		StateMold sm = VALUE(it);
		if (sm->isTerminal) {
			list_push(states_to_transition, sm);
		}
	}

	mold_a->state_count += mold_b->state_count;

	mold_a->states = list_merge(mold_a->states, mold_b->states);
	mold_a->transitions = list_merge(mold_a->transitions, mold_b->transitions);

	// create dummy lists for deallocation
	mold_b->states = list_create();
	mold_b->states = list_create();
	nfa_mold_destroy(mold_b);

	FOREACH(it, states_to_transition) {
		StateMold sm = VALUE(it);
		nfa_mold_addTransition(mold_a, sm->id, "", mold_b_initialState);
		sm->isTerminal = 0;
	}
	list_destroy(states_to_transition);

	NFA result = nfa_mold_compile(mold_a);
	nfa_mold_destroy(mold_a);
	return result;
}

NFA nfa_UNION(NFA a, NFA b) {
	NFAMold mold_a = nfa_decompile(a);
	NFAMold mold_b = nfa_decompile(b);
	NFAMold mold_result = nfa_mold_create();

	unsigned int mold_a_initial = 1;
	FOREACH(it, mold_a->states) {
		StateMold sm = VALUE(it);
		sm->id += 1;
	}
	FOREACH(it, mold_a->transitions) {
		TransitionMold tm = VALUE(it);
		tm->source += 1;
		tm->dest += 1;
	}
	mold_result->states = list_merge(mold_result->states, mold_a->states);
	mold_result->transitions = list_merge(mold_result->transitions, mold_a->transitions);
	mold_result->state_count += mold_a->state_count;

	// create dummy lists for deallocation
	mold_a->states = list_create();
	mold_a->states = list_create();
	nfa_mold_destroy(mold_a);

	unsigned int mold_b_initial = mold_result->state_count;
	FOREACH(it, mold_b->states) {
		StateMold sm = VALUE(it);
		sm->id += mold_result->state_count;
	}
	FOREACH(it, mold_b->transitions) {
		TransitionMold tm = VALUE(it);
		tm->source += mold_result->state_count;
		tm->dest += mold_result->state_count;
	}
	mold_result->states = list_merge(mold_result->states, mold_b->states);
	mold_result->transitions = list_merge(mold_result->transitions, mold_b->transitions);
	mold_result->state_count += mold_b->state_count;

	// create dummy lists for deallocation
	mold_b->states = list_create();
	mold_b->states = list_create();
	nfa_mold_destroy(mold_b);

	nfa_mold_addTransition(mold_result, 0, "", mold_a_initial);
	nfa_mold_addTransition(mold_result, 0, "", mold_b_initial);

	NFA result = nfa_mold_compile(mold_result);
	nfa_mold_destroy(mold_result);
	return result;
}

NFA nfa_KLEENE(NFA a) {
	NFAMold copy_mold = nfa_decompile(a);

	FOREACH(it, copy_mold->states) {
		StateMold sm = VALUE(it);
		if (sm->isTerminal) {
			nfa_mold_addTransition(copy_mold, sm->id, "", 0);
			sm->isTerminal = 0;
		}
	}
	nfa_mold_makeStateTerminal(copy_mold, 0);

	NFA result = nfa_mold_compile(copy_mold);
	nfa_mold_destroy(copy_mold);
	return result;
}

NFA nfa_PLUS(NFA a) {
	NFA kleene = nfa_KLEENE(a);
	NFA result = nfa_UNION(a, kleene);
	nfa_destroy(kleene);
	return result;
}