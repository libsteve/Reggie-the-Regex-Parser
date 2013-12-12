#ifndef REGGIE_H
#define REGGIE_H

#include "nfa.h"

// a structure to represent an NFA coupled with a regular expression
typedef struct reggie_regex {
	char *regex;	// the regular expression string representation
	NFA nfa;		// the NFA that executes the regular expression
} *ReggieRegex;

// create a Reggie Regular Expression
ReggieRegex reggie_create(char *regex);

// destroy a Reggie Regular Expression
void reggie_destroy(ReggieRegex regex);

// get the string representation of the regular expression
char *reggie_getExpression(ReggieRegex regex);

// match a given input string to a regular expression
// returns 1 if the input matches, 0 if the input fails
int reggie_match(ReggieRegex regex, char *input);

// find the first match with a given input string to a regular expression
// returns the length of the first match with the string to the regular expression
int reggie_first(ReggieRegex regex, char *input);

// generate a regular expression that matches for both the given regular expressions
// destroys the two given regular expressions
ReggieRegex reggie_and(ReggieRegex a, ReggieRegex b);

// generate a regular expression that matches for one the given regular expressions
// destroys the two given regular expressions
ReggieRegex reggie_or(ReggieRegex a, ReggieRegex b);

// generate a regular expression that matches for any repeating given regex
// destroys the given regular expression
ReggieRegex reggie_repeat(ReggieRegex regex);

#endif
