#include "regex_parser.h"
#include "nfa_operations.h"

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

Token token_peek(TokenList tokens) {
	return list_rpeek(tokens);
}

NFA parser_pop(RegexParser parser) {
	return list_pop(parser->nfa_stack);
}

void parser_push(RegexParser parser, NFA nfa) {
	list_push(parser->nfa_stack, nfa);
}

// recursive-descent
// based on the grammar defined in regex_grammar.txt

int parse_union(RegexParser parser) {
	if (parser_concat(parser)) {
		Token t = token_pop(parser->tokens);
		if (token_is(t, "|")) {
			if (parse_union(parser)) {
				NFA a = parser_pop(parser);
				NFA b = parser_pop(parser);
				parser_push(parser, nfa_UNION(a, b));
				return 1;
			} else {	
				token_push(parser->tokens, t);
				return 0;
			}
		} else {
			token_push(parser->tokens, t);
			return 1;
		}
	} else {
		return 0;
	}
}

int parse_concat(RegexParser parser) {
	if (parse_term(parser)) {
		if (parse_concat(parser)) {
			NFA a = parser_pop(parser);
			NFA b = parser_pop(parser);
			parser_push(parser, nfa_CONCAT(a, b));
			return 1;
		} else {
			return 1;
		}
	} else {
		return 0;
	}
}

int parse_term(RegexParser parser) {
	if (parse_atom(parser)) {
		if (parse_meta_character(parser)) {
			return 1;
		} else {
			return 1;
		}
	} else {
		return 0;
	}
}

int parse_atom(RegexParser parser) {

}

int parse_meta_character(RegexParser parser) {

}

int parse_character(RegexParser parser) {

}

int parse_character_class(RegexParser parser) {

}