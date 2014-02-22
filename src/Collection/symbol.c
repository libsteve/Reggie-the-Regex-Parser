#include <Collection/symbol.h>
#include <Collection/list.h>
#include <Collection/strings.h>
#include <stdlib.h>

struct symbol_pair {
	char c;
	sym s;
	list symbols;
};

static struct symbol_pair *_next_pair(list symbols, char c) {
	FOREACH(it, symbols) {
		struct symbol_pair *p = VALUE(it);
		if (c == p->c) {
			return p;
		}
	}
	return NULL;
}

sym symbol(char *string) {
	static list symbols = NULL;
	if (!symbols) symbols = list_create();

	list _sym = symbols;
	struct symbol_pair *_p = NULL;
	for (int i=0; i < string_length(string); i++) {
		if (_p) _sym = _p->symbols;
		char c = string[i];
		_p = _next_pair(_sym, c);
		if (_p == NULL) {
			_p = calloc(1, sizeof(struct symbol_pair));
			_p->c = c;
			_p->s = NULL;
			_p->symbols = list_create();
			list_push(_sym, _p);
		}
	}

	if (!_p->s) {
		_p->s = string_copy(string);
	}
	return _p->s;
}
