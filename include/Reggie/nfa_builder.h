#ifndef NFA_BUILDER_H
#define NFA_BUILDER_H

#include "nfa.h"

// parse and build an NFA from a given regular expression string
NFA nfa_build(char *regex);

#endif
