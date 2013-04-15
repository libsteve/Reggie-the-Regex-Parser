#include "regex_tokenizer.h"
#include "regex_tokenizer_nfa.h"
#include "nfa_parsing_eval.h"
#include <stdlib.h>

// a list of token parsers that can be used to parse a regex into different tokens
list token_parsers = 0;

Token_Parser token_parser_create() {
	return calloc(1, sizeof(struct token_parser));
}

void token_parser_destroy(Token_Parser tp) {
	nfa_destroy(tp_parser);
	free(tp);
}

// set up the token parsers if they are not already set up
void setup_token_parsers() {
	if (token_parsers != 0)
		return;

	// atexit(&token_parser_destroy);
	
	token_parsers = list_create();

	// TODO
	// setup instances of the token_parser NFAs and add them to the list
	Token_Parser t;

	t = token_parser_create();
	t->parser = token_nfa_leftParen();
	t->identifier = "(";
	list_push(token_parsers, t);

	t = token_parser_create();
	t->parser = token_nfa_rightParen();
	t->identifier = ")";
	list_push(token_parsers, t);

	t = token_parser_create();
	t->parser = token_nfa_leftSquare();
	t->identifier = "[";
	list_push(token_parsers, t);

	t = token_parser_create();
	t->parser = token_nfa_rightSquare();
	t->identifier = "]";
	list_push(token_parsers, t);

	t = token_parser_create();
	t->parser = token_nfa_pipe();
	t->identifier = "|";
	list_push(token_parsers, t);

	t = token_parser_create();
	t->parser = token_nfa_asterisk();
	t->identifier = "*";
	list_push(token_parsers, t);

	t = token_parser_create();
	t->parser = token_nfa_plus();
	t->identifier = "+";
	list_push(token_parsers, t);

	t = token_parser_create();
	t->parser = token_nfa_escapedChar();
	t->identifier = "ESCAPED";
	list_push(token_parsers, t);

	// classifiers for character classes

	t = token_parser_create();
	t->parser = token_nfa_lowercase();
	t->identifier = "_LOWER";
	list_push(token_parsers, t);

	t = token_parser_create();
	t->parser = token_nfa_uppercase();
	t->identifier = "_UPPER";
	list_push(token_parsers, t);

	t = token_parser_create();
	t->parser = token_nfa_letter();
	t->identifier = "_LETTER";
	list_push(token_parsers, t);

	t = token_parser_create();
	t->parser = token_nfa_digit();
	t->identifier = "_DIGIT";
	list_push(token_parsers, t);

	t = token_parser_create();
	t->parser = token_nfa_whitespace();
	t->identifier = "_WHITESPACE";
	list_push(token_parsers, t);
}

// tear down the token parsers if they are set up
void teardown_token_parsers() {
	if (token_parsers == 0)
		return;

	FOREACH(it, token_parsers) {
		Token_Parser tp = VALUE(it);
		token_parser_destroy(tp);
	}

	list_destroy(token_parsers);
	token_parsers = 0;
}

// create a new token instance
Token token_create() {
	Token t = calloc( 1, sizeof(struct token) );
	return t;
}

// allocate space for a string with the given length
// copy a string of that length from the given string
// set the result to the string attribute of the token
void token_setStringWithLength(Token t, char* string, int length) {
	char* allocated = calloc(length + 1, sizeof(char));

	int i = 0;
	while (i < length && string[i] != '\0') {
		allocated[i] = string[i];
		i++;
	}
	allocated[i] = '\0';

	t->string = allocated;
}

int token_is(Token t, char *identifier) {
	if (t != 0) {
		return string_equals(t->identifier, identifier);
	}
	return 0;
}

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
	char* parsable = regex;

	// tokenize the regex
	while (parsable[0] != '\0') {

		// find something that matches pre-defined stuff
		FOREACH(it, token_parsers) {
			Token_Parser tp = VALUE(it);
			int resulting = nfa_parsing_eval(tp->parser, parsable);
			if (resulting != 0) {
				Token t = token_create();
				t->identifier = tp->identifier;
				token_setStringWithLength(t, parsable, resulting);
				list_push(tl, t);
				resulting += resulting;
				continue;
			}
		}

		// if nothing is found, take the character
		Token t = token_create();
		t->identifier = "CHAR";
		token_setStringWithLength(t, parsable, 1);
		list_push(tl, t);
		resulting += 1;
	}

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

Token tokenlist_pop(TokenList tokens) {
	return list_rpop(tokens);
}

void tokenlist_push(TokenList tokens, Token t) {
	list_rpush(tokens, t);
}

Token tokenlist_peek(TokenList tokens) {
	return list_rpeek(tokens);
}