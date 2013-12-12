#ifndef REGGIE_AUTOMATA_COPY_H
#define REGGIE_AUTOMATA_COPY_H

#include <Reggie/automata.h>

typedef struct {
	automata (*create)();
    state_id (*copyState)(automata destination, state s);
	transition_id (*copyTransition)(automata destination, transition t, state_id newSrc, state_id newDest);
} automata_copy_creation;

automata automata_copy(automata a, automata_copy_creation acc);

#endif
