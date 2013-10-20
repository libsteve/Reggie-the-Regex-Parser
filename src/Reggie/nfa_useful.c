#include "Reggie/nfa_useful.h"
#include "Reggie/nfa_operations.h"

NFA nfa_uppercase() {
	NFA nfa = nfa_create();

	State s = state_create();
	state_setID(s, 1);
	state_makeTerminal(s);

	State q0 = nfa_initialState(nfa);

	for (char c = 'A'; c <= 'Z'; c++)
	{
		char str[] = {c, '\0'};
		state_addTransition(q0, str, s);
	}

	nfa_addState(nfa, s);

	return nfa;
}

NFA nfa_lowercase() {
	NFA nfa = nfa_create();

	State s = state_create();
	state_setID(s, 1);
	state_makeTerminal(s);

	State q0 = nfa_initialState(nfa);

	for (char c = 'a'; c <= 'z'; c++)
	{
		char str[] = {c, '\0'};
		state_addTransition(q0, str, s);
	}

	nfa_addState(nfa, s);

	return nfa;
}

NFA nfa_letter() {
	return nfa_UNION(nfa_uppercase(), nfa_lowercase());
}

NFA nfa_digit() {
	NFA nfa = nfa_create();

	State s = state_create();
	state_setID(s, 1);
	state_makeTerminal(s);

	State q0 = nfa_initialState(nfa);

	for (char c = '0'; c <= '9'; c++)
	{
		char str[] = {c, '\0'};
		state_addTransition(q0, str, s);
	}

	nfa_addState(nfa, s);

	return nfa;
}

NFA nfa_whitespace() {
	NFA nfa = nfa_create();

	State s = state_create();
	state_setID(s, 1);
	state_makeTerminal(s);

	State q0 = nfa_initialState(nfa);

	state_addTransition(q0, " ", s);
	state_addTransition(q0, "\t", s);
	state_addTransition(q0, "\n", s);

	nfa_addState(nfa, s);

	return nfa;
}