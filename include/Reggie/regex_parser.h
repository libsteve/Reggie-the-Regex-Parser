#ifndef REGEX_PARSER_H
#define REGEX_PARSER_H

#include "regex_tokenizer.h"
#include "nfa.h"
#include "Collection/list.h"

// a structure for use with parsing a string into a regular expression
typedef struct regex_parser {
	TokenList tokens;	// a list of tokens
	TokenList used;		// a stack of used tokens
	list nfa_stack;		// a stack of NFAs for use with recursive-descent parsers
	NFA result;			// an NFA resulting from the token list
} *RegexParser;

// create a new RegexParser structure instance
RegexParser parser_create();

// destroy a RegexParser instance
void parser_destroy(RegexParser parser);

// set the tokens of a regex parser instance
void parser_setTokens(RegexParser parser, TokenList tl);

// get the resulting NFA from the regex parser
NFA parser_getNFA(RegexParser parser);

// rewind the used/tokens lists in the parser 
// until the used is empty or the token t is on the used
void parser_rewindTo(RegexParser parser, Token t);

// push an nfa onto the nfa_stack
void parser_push(RegexParser parser, NFA nfa);

// pop an nfa off the nfa_stack
NFA parser_pop(RegexParser parser);

// peek at an nfa in the nfa_stack
NFA parser_peek(RegexParser parser);

///////////////////////////////////////////////////////
// recursive-descent
// based on the grammar defined in regex_grammar.txt
///////////////////////////////////////////////////////

// a typedef for any function that parses on a RegexParser object
// the return type is 1 if the parser succeeds, and 0 if the parser fails
// these functions are recursive-descent-style parsers
typedef int (*parse_tokens)(RegexParser);

// parse for the union grammar
int parse_union(RegexParser parser);

// parse for the concat grammar
int parse_concat(RegexParser parser);

// parse for the term grammar
int parse_term(RegexParser parser);

// parse for the atom grammar
int parse_atom(RegexParser parser);

// parse for the meta_character grammar
int parse_meta_character(RegexParser parser);

// parse for the character grammar
int parse_character(RegexParser parser);

// parse for the character_class grammar
int parse_character_class(RegexParser parser);

#endif
