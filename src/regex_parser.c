#include "regex_parser.h"

// helpers

int token_is(Token t, char *identifier) {
	if (t != 0) {
		return string_equals(t->identifier, identifier);
	}
	return 0;
}

Token token_pop(TokenList tokens) {
	return list_rpop(tokens);
}

void token_unpop(TokenList tokens, Token t) {
	list_rpush(tokens, t);
}

NFA parser_pop(RegexParser parser) {
	return list_pop(parser->nfa_stack);
}

void parser_push(RegexParser parser, NFA nfa) {
	list_push(parser->nfa_stack, nfa);
}

// recursive-descent

int parse_union(RegexParser parser) {
	if (parser_concat(parser)) {
		Token t = token_pop(parser->tokens);
		if (token_is(t, "|")) {
			if (parse_union(parser)) {
				NFA a = parser_pop(parser);
				NFA b = parser_pop(parser);
			}
		}
	}

	return 0;
}

int parse_concat(RegexParser parser) {

}

int parse_term(RegexParser parser) {

}

int parse_atom(RegexParser parser) {

}

int parse_meta_character(RegexParser parser) {

}

int parse_character(RegexParser parser) {

}

int parse_character_class(RegexParser parser) {

}