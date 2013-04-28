#ifndef NFA_COPY_H
#define NFA_COPY_H

#include "map.h"
#include "nfa.h"
#include "nfa_create.h"

// generate an nfa mold from a given nfa
NFAMold nfa_mold_createFromNFA(NFA nfa);

#define nfa_decompile(nfa) (nfa_mold_createFromNFA((nfa)))

// create a copy of an nfa from a given nfa
NFA nfa_copy(NFA nfa);

#endif