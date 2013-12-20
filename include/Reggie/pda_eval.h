#ifndef REGGIE_PDA_EVAL_H
#define REGGIE_PDA_EVAL_H

#include <Reggie/pda.h>

// evaluate the pda with the given input string
// returns true if the input string matches the pda, false otherwise
bool pda_eval(PDA pda, char* input);

// evaluate the pda with the given input string
// returns -1 if failure or the length of the first successful substring match
int pda_parsing_eval(PDA pda, char* input);

#endif

