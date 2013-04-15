#include "nfa_builder.h"
#include "regex_parser.h"
#include "regex_tokenizer.h"

// parse and build an NFA from a given regular expression string
NFA nfa_build(char *regex) {
	TokenList tokens = regex_tokenize(regex);
	RegexParser parser = parser_create();
	parser_setTokens(parser, tokens);
	int ok = parse_union(parser);
	if (ok) {
		NFA result = parse_getNFA(parser);
		parser_destroy(parser);
		return NFA;
	}
	parser_destroy(parser);
	return 0;
}