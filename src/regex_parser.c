#include "regex_parser.h"
#include "nfa_operations.h"
#include "nfa_useful.h"

RegexParser parser_create() {
	RegexParser parser = calloc(1, sizeof(struct regex_parser));
	parser->used = list_create();
	parser->nfa_stack = list_create();
	return parser;
}

void parser_destroy(RegexParser parser) {
	if (parser->tokens != 0)
		tokenlist_destroy(parser->tokens);
	tokenlist_destroy(parser->used);
	FOREACH(it, parser->nfa_stack) {
		nfa_destroy(VALUE(it));
	}
	list_destroy(parser->nfa_stack);
	free(parser);
}

void parser_setTokens(RegexParser parser, TokenList tl) {
	parser->tokens = tl;
}

NFA parser_getNFA(RegexParser parser) {
	return parser_pop(parser);
}

// rewind the used/tokens lists in the parser 
// until the used is empty or the token t is on the used
void parser_rewindTo(RegexParser parser, Token t) {
	Token c = 0;
	while (tokenlist_peek(parser->used) != t) {
		c = tokenlist_pop(parser->used);
		tokenlist_push(parser->tokens, c);
	}
}

void parser_push(RegexParser parser, NFA nfa) {
	list_push(parser->nfa_stack, nfa);
}

NFA parser_pop(RegexParser parser) {
	return list_pop(parser->nfa_stack);
}

NFA parser_peek(RegexParser parser) {
	return list_peek(parser->nfa_stack);
}

///////////////////////////////////////////////////////
// recursive-descent
// based on the grammar defined in regex_grammar.txt
///////////////////////////////////////////////////////

int parse_union(RegexParser parser) {
	if (parser_concat(parser)) {
		Token t = tokenlist_pop(parser->tokens);
		tokenlist_push(parser->used, t);
		if (token_is(t, "|")) {
			if (parse_union(parser)) {
				NFA a = parser_pop(parser);
				NFA b = parser_pop(parser);
				parser_push(parser, nfa_UNION(a, b));
				return 1;
			} else {
				tokenlist_pop(parser->used);
				tokenlist_push(parser->tokens, t);
				return 0;
			}
		} else {
			tokenlist_pop(parser->used);
			tokenlist_push(parser->tokens, t);
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
	Token t = tokenlist_pop(parser->tokens);
	tokenlist_push(parser->used, t);

	if (token_is(t, "(")) {
		if (parse_union(parser)) {
			Token u = tokenlist_pop(parser->tokens);
			tokenlist_push(parser->used, u);
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
			Token u = tokenlist_pop(parser->tokens);
			tokenlist_push(parser->used, u);
			if (token_is(t, "]")) {
				return 1;
			} else {
				NFA a = parser_pop(parser);
				nfa_destroy(a);
			}
		}
		parser_rewindTo(parser, t);
	}

	tokenlist_pop(parser->used);
	tokenlist_push(parser->tokens, t);
	if (parse_character(parser)) {
		return 1;
	}

	return 0;
}

int parse_meta_character(RegexParser parser) {
	Token t = tokenlist_pop(parser->tokens);
	tokenlist_push(parser->used, t);

	if (token_is(t, "*")) {
		NFA a = parser_pop(parser);
		a = nfa_KLEENE(a);
		parser_push(a);
		return 1;
	} else if (token_is(t, "+")) {
		// not implementable due to inability to copy NFAs
		// return 1;
	}

	tokenlist_push(parser->tokens, t);
	tokenlist_pop(parser->used);
	return 0;
}

int parse_character(RegexParser parser) {
	Token t = tokenlist_pop(parser->tokens);
	tokenlist_push(parser->used, t);

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

	tokenlist_push(parser->tokens, t);
	tokenlist_pop(parser->used);
	return 0;
}

int parse_character_class(RegexParser parser) {
	Token t = tokenlist_pop(parser->tokens);
	tokenlist_push(parser->used, t);

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

	tokenlist_push(parser->tokens, t);
	tokenlist_pop(parser->used);
	return 0;
}

int parse_escaped(RegexParser parser) {
	Token t = tokenlist_pop(parser->tokens);
	tokenlist_push(parser->used, t);
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
	tokenlist_push(parser->tokens, t);
	tokenlist_pop(parser->used);
	return 0;
}