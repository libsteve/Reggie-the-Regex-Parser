#include <Reggie/regex_tokenizer_nfa.h>
#include <Reggie/nfa_useful.h>
#include <Reggie/nfa_operations.h>

NFA token_nfa_leftParen() {
	NFA nfa = nfa_create();

	state_id s = nfa_addState(nfa);
	state_id q0 = nfa_initialState(nfa);

	nfa_addTransition(nfa, q0, s, "(");

	return nfa;
}

NFA token_nfa_rightParen() {
	NFA nfa = nfa_create();

	state_id s = nfa_addState(nfa);
	state_id q0 = nfa_initialState(nfa);

	nfa_addTransition(nfa, q0, s, ")");

	return nfa;
}

NFA token_nfa_leftSquare() {
	NFA nfa = nfa_create();

	state_id s = nfa_addState(nfa);
	state_id q0 = nfa_initialState(nfa);

	nfa_addTransition(nfa, q0, s, "[");

	return nfa;
}

NFA token_nfa_rightSquare() {
	NFA nfa = nfa_create();

	state_id s = nfa_addState(nfa);
	state_id q0 = nfa_initialState(nfa);

	nfa_addTransition(nfa, q0, s, "]");

	return nfa;
}

NFA token_nfa_pipe() {
	NFA nfa = nfa_create();

	state_id s = nfa_addState(nfa);
	state_id q0 = nfa_initialState(nfa);

	nfa_addTransition(nfa, q0, s, "|");

	return nfa;
}

NFA token_nfa_asterisk() {
	NFA nfa = nfa_create();

	state_id s = nfa_addState(nfa);
	state_id q0 = nfa_initialState(nfa);

	nfa_addTransition(nfa, q0, s, "*");

	return nfa;
}

NFA token_nfa_plus() {
	NFA nfa = nfa_create();

	state_id s = nfa_addState(nfa);
	state_id q0 = nfa_initialState(nfa);

	nfa_addTransition(nfa, q0, s, "+");

	return nfa;
}

NFA token_nfa_escapedChar() {
	NFA nfa = nfa_create();

	state_id s = nfa_addState(nfa);
	state_id q0 = nfa_initialState(nfa);

	nfa_addTransition(nfa, q0, s, "/(");
	nfa_addTransition(nfa, q0, s, "/)");
	nfa_addTransition(nfa, q0, s, "/|");
	nfa_addTransition(nfa, q0, s, "/*");
	nfa_addTransition(nfa, q0, s, "/+");
	nfa_addTransition(nfa, q0, s, "/[");
	nfa_addTransition(nfa, q0, s, "/]");
	// nfa_addTransition(nfa, q0, s, "//");

	return nfa;
}

// classifiers for character classes

NFA token_nfa_lowercase() {
	NFA nfa = nfa_create();

	state_id s = nfa_addState(nfa);
	state_id q0 = nfa_initialState(nfa);

	nfa_addTransition(nfa, q0, s, ":lower:");

	return nfa;
}

NFA token_nfa_uppercase() {
	NFA nfa = nfa_create();

	state_id s = nfa_addState(nfa);
	state_id q0 = nfa_initialState(nfa);

	nfa_addTransition(nfa, q0, s, ":upper:");

	return nfa;
}

NFA token_nfa_letter() {
	NFA nfa = nfa_create();

	state_id s = nfa_addState(nfa);
	state_id q0 = nfa_initialState(nfa);

	nfa_addTransition(nfa, q0, s, ":letter:");

	return nfa;
}

NFA token_nfa_digit() {
	NFA nfa = nfa_create();

	state_id s = nfa_addState(nfa);
	state_id q0 = nfa_initialState(nfa);

	nfa_addTransition(nfa, q0, s, ":digit:");

	return nfa;
}

NFA token_nfa_whitespace() {
	NFA nfa = nfa_create();

	state_id s = nfa_addState(nfa);
	state_id q0 = nfa_initialState(nfa);

	nfa_addTransition(nfa, q0, s, ":wspace:");

	return nfa;
}

