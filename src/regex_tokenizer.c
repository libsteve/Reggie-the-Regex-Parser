#include "regex_tokenizer.h"

// a list of token parsers that can be used to parse a regex into different tokens
list token_parsers = 0;

// set up the token parsers if they are not already set up
void setup_token_parsers() {
	if (token_parsers != 0)
		teardown_token_parsers();
	token_parsers = list_create();

	// TODO
	// setup instances of the token_parser NFAs and add them to the list
}

// tear down the token parsers if they are set up
void teardown_token_parsers() {
	if (token_parsers == 0)
		return;

	FOREACH(it, token_parsers) {
		NFA parser = VALUE(it);
		nfa_destroy(parser);
	}

	list_destroy(token_parsers);
}

// create a new token instance
Token token_create() {
	Token t = calloc( 1, sizeof(struct token) );
	return t;
}

// allocate space for a string with the given length
// copy a string of that length from the given string
// set the result to the string attribute of the token
void token_setStringWithLength(Token t, char* string, int length);

// deallocate the string
// deallocate the token
void token_destroy(Token t) {
	free(t->string);
	free(t);
}

// tokenize a regular expression
// returns a list of tokens in the order they appear
TokenList regex_tokenize(char* regex) {
	TokenList tl = list_create();

	// TODO
	// tokenize the regex

	return tl;
}

// destroy a token list and all it's tokens
void tokenlist_destroy(TokenList tl) {
	FOREACH(it, tl) {
		Token t = VALUE(it);
		token_destroy(t);
	}

	list_destroy(tl);
}