#include "nfa_copy.h"
#include "strings.h"

// generate an nfa mold from a given nfa
NFAMold nfa_mold_createFromNFA(NFA nfa) {
	NFA mold = nfa_mold_create();
	// copy the initial state
	// for each state in the nfa that isn't the initial state, copy it
	// make copies of each transition and attach them to the correct copies states
	return mold;
}

// create a copy of an nfa from a given nfa
NFA nfa_copy(NFA nfa) {
	NFAMold mold = nfa_mold_createFromNFA(nfa);
	NFA copy = nfa_createFromMold(NFAMold mold);
	nfa_mold_destroy(mold);
	return copy;
}