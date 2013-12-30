#ifndef REGGIE_PDA_EVAL_H
#define REGGIE_PDA_EVAL_H

#include <Reggie/pda.h>

typedef struct pda_eval_result {
    bool success;
    int length;
    void *data;
    void (*data_destroy)(void *data);
} pda_eval_result;

// evaluate the pda with the given input string
// returns true if the input string matches the pda, false otherwise
bool pda_eval(PDA pda, char* input);

// evaluate the pda with the given input string
// returns -1 if failure or the length of the first successful substring match
int pda_parsing_eval(PDA pda, char* input);

// evaluate the pda with the given input string
// returns a structure with:
//      a success of false if failure or ture otherwise
//      a length of -1 if failure or the first successful substring match
//      a data of NULL if failure or the successful data result
//      a data_destroy function to properly destroy the data result or NULL
pda_eval_result pda_data_eval(PDA pda, char *input);

#endif

