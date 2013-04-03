#ifndef REGEX_PARSER_H
#define REGEX_PARSER_H

#include "regex_tokenizer.h"
#include "nfa.h"
#include "list.h"

typedef struct regex_parser {
	TokenList tokens;
	NFA result;
} *RegexParser;

#endif