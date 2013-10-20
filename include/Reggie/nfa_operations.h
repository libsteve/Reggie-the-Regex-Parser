#ifndef NFA_OPERATIONS_H
#define NFA_OPERATIONS_H

#include "nfa.h"

// take two NFAs and concatenate them together into a new NFA.
NFA nfa_CONCAT(NFA a, NFA b);

// take two NFAs and union them together into a new NFA.
NFA nfa_UNION(NFA a, NFA b);

// take an NFA and kleene star it into a new NFA.
NFA nfa_KLEENE(NFA a);

// take an NFA and concatenate it with it's kleene star
NFA nfa_PLUS(NFA a);

#endif