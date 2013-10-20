#include "Reggie/reggie.h"
#include "Collection/strings.h"
#include "Reggie/nfa.h"
#include "Reggie/nfa_eval.h"
#include "Reggie/nfa_builder.h"
#include "Reggie/nfa_operations.h"
#include "Reggie/nfa_parsing_eval.h"
#include <stdlib.h>

// create a Reggie Regular Expression
ReggieRegex reggie_create(char *regex) {
	NFA nfa = nfa_build(regex);
	if (nfa) {
		ReggieRegex r = calloc(1, sizeof(struct reggie_regex));
		r->nfa = nfa;
		r->regex = string_copy(regex);
		return r;
	}
	return 0;
}

// destroy a Reggie Regular Expression
void reggie_destroy(ReggieRegex regex) {
	if (regex->nfa)
		nfa_destroy(regex->nfa);
	free(regex->regex);
	free(regex);
}

// match a given input string to a regular expression
// returns 1 if the input matches, 0 if the input fails
int reggie_match(ReggieRegex regex, char *input) {
	return nfa_eval(regex->nfa, input);
}

// find the first match with a given input string to a regular expression
// returns the length of the first match with the string to the regular expression
int reggie_first(ReggieRegex regex, char *input) {
	return nfa_parsing_eval(regex->nfa, input);
}

// generate a regular expression that matches for both the given regular expressions
// destroys the two given regular expressions
ReggieRegex reggie_and(ReggieRegex a, ReggieRegex b) {
	ReggieRegex r = calloc(1, sizeof(struct reggie_regex));	

	char *x = string_concat("(", a->regex);
	char *y = string_concat(x, ")(");
	char *z = string_concat(y, b->regex);
	r->regex = string_concat(z, ")");
	free(x);
	free(y);
	free(z);

	r->nfa = nfa_CONCAT(a->nfa, b->nfa);
	reggie_destroy(a);
	reggie_destroy(b);

	return r;
}

// generate a regular expression that matches for one the given regular expressions
// destroys the two given regular expressions
ReggieRegex reggie_or(ReggieRegex a, ReggieRegex b) {
	ReggieRegex r = calloc(1, sizeof(struct reggie_regex));	

	char *x = string_concat("(", a->regex);
	char *y = string_concat(x, ")|(");
	char *z = string_concat(y, b->regex);
	r->regex = string_concat(z, ")");
	free(x);
	free(y);
	free(z);

	r->nfa = nfa_UNION(a->nfa, b->nfa);
	reggie_destroy(a);
	reggie_destroy(b);

	return r;
}

// generate a regular expression that matches for any repeating given regex
// destroys the given regular expression
ReggieRegex reggie_repeat(ReggieRegex regex) {
	ReggieRegex r = calloc(1, sizeof(struct reggie_regex));	

	char *x = string_concat("(", regex->regex);
	r->regex = string_concat(x, ")*");
	free(x);

	r->nfa = nfa_KLEENE(regex->nfa);
	reggie_destroy(regex);

	return r;
}