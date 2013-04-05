#include "regex_parser.h"
#include "nfa_operations.h"
#include "nfa_useful.h"

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

// rewind the used/tokens lists in the parser 
// until the used is empty or the token t is on the used
void parser_rewindTo(RegexParser parser, Token t) {
	Token c = 0;
	while (token_peek(parser->used) != t) {
		c = token_pop(parser->used);
		token_push(parser->tokens, c);
	}
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
		token_push(parser->used, t);
		if (token_is(t, "|")) {
			if (parse_union(parser)) {
				NFA a = parser_pop(parser);
				NFA b = parser_pop(parser);
				parser_push(parser, nfa_UNION(a, b));
				return 1;
			} else {
				token_pop(parser->used);
				token_push(parser->tokens, t);
				return 0;
			}
		} else {
			token_pop(parser->used);
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
	Token t = token_pop(parser->tokens);
	token_push(parser->used, t);

	if (token_is(t, "(")) {
		if (parse_union(parser)) {
			Token u = token_pop(parser->tokens);
			token_push(parser->used, u);
			if (token_is(t, ")")) {
				return 1;
			} else {
				NFA a = parser_pop(parser);
				nfa_destroy(a);
			}
		}
		parser_rewindTo(parser, t);
	}

	if (token_is(t, "[")) {
		if (parse_character_class(parser)) {
			Token u = token_pop(parser->tokens);
			token_push(parser->used, u);
			if (token_is(t, "]")) {
				return 1;
			} else {
				NFA a = parser_pop(parser);
				nfa_destroy(a);
			}
		}
		parser_rewindTo(parser, t);
	}

	token_pop(parser->used);
	token_push(parser->tokens, t);
	if (parse_character(parser)) {
		return 1;
	}

	return 0;
}

int parse_meta_character(RegexParser parser) {
	Token t = token_pop(parser->tokens);
	token_push(parser->used, t);

	if (token_is(t, "*")) {
		NFA a = parser_pop(parser);
		a = nfa_KLEENE(a);
		parser_push(a);
		return 1;
	} else if (token_is(t, "+")) {
		// not implementable due to inability to copy NFAs
		// return 1;
	}

	token_push(parser->tokens, t);
	token_pop(parser->used);
	return 0;
}

int parse_character(RegexParser parser) {
	Token t = token_pop(parser->tokens);
	token_push(parser->used, t);

	if (token_is(t, "CHAR")) {
		// create nfa for the char
		State s = state_create();
		state_makeTerminal(s);
		NFA nfa = nfa_create();
		state_addTransition(nfa_initialState(nfa), t->string, s);

		// push the resulting nfa to the parser
		parser_push(nfa);
		return 1;
	}

	token_push(parser->tokens, t);
	token_pop(parser->used);
	return 0;
}

int parse_character_class(RegexParser parser) {
	Token t = token_pop(parser->tokens);
	token_push(parser->used, t);

	if (token_is(t, "_LETTER")) {
		parser_push(parser, nfa_letter());
		return 1;
	} else if (token_is(t, "_UPPER")) {
		parser_push(parser, nfa_uppercase());
		return 1;
	} else if (token_is(t, "_LOWER")) {
		parser_push(parser, nfa_lowercase());
		return 1;
	} else if (token_is(t, "_DIGIT")) {
		parser_push(parser, nfa_digit());
		return 1;
	} else if (token_is(t, "_WHITESPACE")) {
		parser_push(parser, nfa_whitespace());
		return 1;
	}

	token_push(parser->tokens, t);
	token_pop(parser->used);
	return 0;
}

int parse_escaped(RegexParser parser) {
	Token t = token_pop(parser->tokens);
	token_push(parser->used, t);
	if (token_is(t, "ESCAPED")) {
		// create nfa for the char
		char *c = t->string + 1;
		State s = state_create();
		state_makeTerminal(s);
		NFA nfa = nfa_create();
		state_addTransition(nfa_initialState(nfa), c, s);

		// push the resulting nfa to the parser
		parser_push(nfa);
		return 1;
	}
	token_push(parser->tokens, t);
	token_pop(parser->used);
	return 0;
}