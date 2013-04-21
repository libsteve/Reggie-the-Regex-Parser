#ifndef REGEX_TOKENIZER_H
#define REGEX_TOKENIZER_H

#include "nfa.h"
#include "list.h"

// a structure to represent a token parser
typedef struct token_parser {
	NFA 	parser;		// an NFA to parse the tokens
	char* 	identifier;	// an string to identify the resulting token's type
} *Token_Parser;

// a list of token parsers that can be used to parse a regex into different tokens
extern list token_parsers;

// set up the token parsers if they are not already set up
void setup_token_parsers();

// tear down the token parsers if they are set up
void teardown_token_parsers();

// a structure to represent a parsed token
typedef struct token {
	char* 	string;		// an allocated string of what is parsed
	char* 	identifier;	// an identifying string for the token type
} *Token;

// create a new token instance
Token token_create();

// allocate space for a string with the given length
// copy a string of that length from the given string
// set the result to the string attribute of the token
void token_setStringWithLength(Token t, char* string, int length);

// identify whether or not the given token 
// is a type of the given identifier string.
// returns 1 if the identifier and token match, 0 otherwise
int token_is(Token t, char *identifier);

// deallocate the string
// deallocate the token
void token_destroy(Token t);

// a typedef of list to TokenList for readability's sake
typedef list TokenList;

// tokenize a regular expression
// returns a list of tokens in the order they appear
TokenList regex_tokenize(char* regex);

// destroy a token list and all it's tokens
void tokenlist_destroy(TokenList tl);

// push a token onto the beginning of a token list
void tokenlist_push(TokenList tokens, Token t);

// look at the beginning of a token list
Token tokenlist_peek(TokenList tokens);

// pop a token off the beginning of a token list
Token tokenlist_pop(TokenList tokens);

#endif