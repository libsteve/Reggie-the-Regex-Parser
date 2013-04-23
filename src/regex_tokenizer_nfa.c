#include "regex_tokenizer_nfa.h"
#include "nfa_useful.h"
#include "nfa_operatons.h"

NFA token_nfa_leftParen() {
	NFA nfa = nfa_create();

	State s = state_create();
	State q0 = nfa_initialState(nfa);

	state_addTransition(q0, "(", s);

	return nfa;
}

NFA token_nfa_rightParen() {
	NFA nfa = nfa_create();

	State s = state_create();
	State q0 = nfa_initialState(nfa);

	state_addTransition(q0, ")", s);

	return nfa;
}

NFA token_nfa_leftSquare() {
	NFA nfa = nfa_create();

	State s = state_create();
	State q0 = nfa_initialState(nfa);

	state_addTransition(q0, "[", s);

	return nfa;
}

NFA token_nfa_rightSquare() {
	NFA nfa = nfa_create();

	State s = state_create();
	State q0 = nfa_initialState(nfa);

	state_addTransition(q0, "]", s);

	return nfa;
}

NFA token_nfa_pipe() {
	NFA nfa = nfa_create();

	State s = state_create();
	State q0 = nfa_initialState(nfa);

	state_addTransition(q0, "|", s);

	return nfa;
}

NFA token_nfa_asterisk() {
	NFA nfa = nfa_create();

	State s = state_create();
	State q0 = nfa_initialState(nfa);

	state_addTransition(q0, "*", s);

	return nfa;
}

NFA token_nfa_plus() {
	NFA nfa = nfa_create();

	State s = state_create();
	State q0 = nfa_initialState(nfa);

	state_addTransition(q0, "+", s);

	return nfa;
}

NFA token_nfa_escapedChar() {
	NFA nfa = nfa_create();

	State s = state_create();
	State q0 = nfa_initialState(nfa);

	state_addTransition(q0, "/(", s);
	state_addTransition(q0, "/)", s);
	state_addTransition(q0, "/|", s);
	state_addTransition(q0, "/*", s);
	state_addTransition(q0, "/+", s);
	state_addTransition(q0, "/[", s);
	state_addTransition(q0, "/]", s);
	// state_addTransition(q0, "//", s);

	return nfa;
}

// classifiers for character classes

NFA token_nfa_lowercase() {
	NFA nfa = nfa_create();

	State s = state_create();
	State q0 = nfa_initialState(nfa);

	state_addTransition(q0, ":lower:", s);

	return nfa;
}

NFA token_nfa_uppercase() {
	NFA nfa = nfa_create();

	State s = state_create();
	State q0 = nfa_initialState(nfa);

	state_addTransition(q0, ":upper:", s);

	return nfa;
}

NFA token_nfa_letter() {
	NFA nfa = nfa_create();

	State s = state_create();
	State q0 = nfa_initialState(nfa);

	state_addTransition(q0, ":letter:", s);

	return nfa;
}

NFA token_nfa_digit() {
	NFA nfa = nfa_create();

	State s = state_create();
	State q0 = nfa_initialState(nfa);

	state_addTransition(q0, ":digit:", s);

	return nfa;
}

NFA token_nfa_whitespace() {
	NFA nfa = nfa_create();

	State s = state_create();
	State q0 = nfa_initialState(nfa);

	state_addTransition(q0, ":wspace:", s);

	return nfa;
}

