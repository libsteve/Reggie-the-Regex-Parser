#include "Reggie/nfa_useful.h"
#include "Reggie/nfa_operations.h"

NFA nfa_uppercase() {
	NFA nfa = nfa_create();
	
	state_id s = nfa_addState(nfa);
	nfa_state_makeTerminal(nfa, s);

	state_id q0 = nfa_initialState(nfa);

	for (char c = 'A'; c <= 'Z'; c++)
	{
		char str[] = {c, '\0'};
		nfa_addTransition(nfa, q0, s, str);
	}

	return nfa;
}

NFA nfa_lowercase() {
	NFA nfa = nfa_create();

	state_id s = nfa_addState(nfa);
	nfa_state_makeTerminal(nfa, s);

	state_id q0 = nfa_initialState(nfa);

	for (char c = 'a'; c <= 'z'; c++)
	{
		char str[] = {c, '\0'};
		nfa_addTransition(nfa, q0, s, str);
	}

	return nfa;
}

NFA nfa_letter() {
	return nfa_UNION(nfa_uppercase(), nfa_lowercase());
}

NFA nfa_digit() {
	NFA nfa = nfa_create();

	state_id s = nfa_addState(nfa);
	nfa_state_makeTerminal(nfa, s);

	state_id q0 = nfa_initialState(nfa);

	for (char c = '0'; c <= '9'; c++)
	{
		char str[] = {c, '\0'};
		nfa_addTransition(nfa, q0, s, str);
	}

	return nfa;
}

NFA nfa_whitespace() {
	NFA nfa = nfa_create();

	state_id s = nfa_addState(nfa);
	nfa_state_makeTerminal(nfa, s);

	state_id q0 = nfa_initialState(nfa);

	nfa_addTransition(nfa, q0, s, " ");
	nfa_addTransition(nfa, q0, s, "\t");
	nfa_addTransition(nfa, q0, s, "\n");

	return nfa;
}