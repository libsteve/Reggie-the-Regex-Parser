#ifndef REGGIE_AUTOMATA_OPERATIONS_H
#define REGGIE_AUTOMATA_OPERATIONS_H

#include <Reggie/automata.h>

typedef struct {
	automata (*create)();
	state_id (*addState)(automata a);
	transition_id (*addEpsilonTransition)(automata a, state_id src, state_id dest);
	transition_id (*copyTransition)(automata destination, transition t, state_id newSrc, state_id newDest);
} automata_creation;

automata automata_concat(automata a, automata b, automata_creation creation);

automata automata_union(automata a, automata b, automata_creation creation);

automata automata_kleene(automata a, automata_creation creation);

automata automata_plus(automata a, automata_creation creation);

#endif