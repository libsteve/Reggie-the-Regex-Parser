#ifndef REGGOE_AUTOMATA_EVAL_H
#define REGGOE_AUTOMATA_EVAL_H

#include <Reggie/automata.h>

//////
// total evaluations of automata

// returns true if the input string matches the automata, false otherwise
bool automata_eval(automata a, struct stream input);
bool state_eval(automata a, state s, struct stream input);
bool transition_eval(automata a, transition t, struct stream input);

//////
// partial parsing evaluations of automata

// returns -1 if failure or the length of the first successful substring match
int automata_parsing_eval(automata a, struct stream input);
int state_parsing_eval(automata a, state s, struct stream input);
int transition_parsing_eval(automata a, transition t, struct stream input);

#endif
