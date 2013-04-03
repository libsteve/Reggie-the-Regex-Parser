#ifndef REGEX_PARSER_H
#define REGEX_PARSER_H

#include "regex_tokenizer.h"
#include "nfa.h"
#include "list.h"

// a structure for use with parsing a string into a regular expression
typedef struct regex_parser {
	TokenList tokens;	// a list of tokens
	List nfa_list;		// a stack of NFAs for use with recursive-descent parsers
	NFA result;			// an NFA resulting from the token list
} *RegexParser;

// a typedef for any function that parses on a RegexParser object
// the return type is 1 if the parser succeeds, and 0 if the parser fails
// these functions are recursive-descent-style parsers
typedef int (*parse_tokens)(RegexParser);

int parse_union(RegexParser parser);

int parse_concet(RegexParser parser);

int parse_term(RegexParser parser);

int parse_atom(RegexParser parser);

int parse_meta_character(RegexParser parser);

int parse_character(RegexParser parser);

int parse_character_class(RegexParser parser);

#endif