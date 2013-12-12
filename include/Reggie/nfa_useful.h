#ifndef NFA_USEFUL_H
#define NFA_USEFUL_H

#include "nfa.h"

// generate an NFA that identifies an upper-case character
NFA nfa_uppercase();

// generate an NFA that identifies a lower-case character
NFA nfa_lowercase();

// generate an NFA that identifies any letter character
NFA nfa_letter();

// generate an NFA that identifies a digit
NFA nfa_digit();

// generate an NFA that identifies whitespace
NFA nfa_whitespace();

#endif
