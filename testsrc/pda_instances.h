#ifndef REGGIE_TEST_PDA_INSTANCES_H
#define REGGIE_TEST_PDA_INSTANCES_H

#include <Reggie/pda.h>
#include <Reggie/nfa.h>

inline PDA empty_rejecting_pda() {
    PDA p = pda_create();
    return p;
}

inline PDA empty_accepting_pda() {
    PDA p = empty_rejecting_pda();
    pda_state_makeTerminal(p, pda_initialState(p));
    return p;
}

inline PDA one_string_transition_pda() {
    PDA p = empty_rejecting_pda();
    state_id q0 = pda_initialState(p);
    state_id q1 = pda_addState(p);
    pda_state_makeTerminal(p, q1);
    pda_addStringTransition(p, q0, q1, "a", pda_dont_pop, pda_dont_push);
    return p;
}

inline PDA one_NFA_transition_pda() {
    PDA p = empty_rejecting_pda();
    state_id q0 = pda_initialState(p);
    state_id q1 = pda_addState(p);
    pda_state_makeTerminal(p, q1);

    NFA n = nfa_create();
    state_id nq0 = nfa_initialState(n);
    state_id nq1 = nfa_addState(n);
    nfa_state_makeTerminal(n, nq1);
    nfa_addTRansition(n, nq0, nq1, "a");

    pfa_addNFATransition(p, q0, q1, n, pda_dont_pop, pda_dont_push);

    nfa_destroy(n);

    return p;
}

inline PDA balanced_parenthesis_pda() {
    PDA p = empty_rejecting_pda();
    state_id q0 = pda_initialState(p);
    state_id qloop = pda_addState(p);
    state_id qsuccess = pda_addState(p);
    pda_state_makeTerminal(p, qsuccess);

    // Context Free Grammar:
    // S := ( S )
    // S := ()

    enum {
        S_RULE,     // S
        UNIT,       // ()
        LEFT_PAREN, // (
        RIGHT_PAREN // )
    };

    // initialize PDA with the S rule
    pda_addStringTransition(p, q0, qloop, "", pda_dont_pop, S_RULE);

    // S := ( S )   (tokens pushed in reverse order)
    state_id qS1 = pda_addState(p);
    state_id qS2 = pda_addState(p);
    pda_addStringTransition(p, qloop, qS1, "", S_RULE, RIGHT_PAREN);
    pda_addStringTransition(p, qS1, qS2, "", pda_dont_pop, S_RULE);
    pda_addStringTransition(p, qS2, qloop, "", pda_dont_pop, LEFT_PAREN);

    // S := ()
    pda_addStringTransition(p, qloop, qloop, "", S_RULE, UNIT);

    // () token
    pda_addStringTransition(p, qloop, qloop, "()", UNIT, pda_dont_push);

    // ( token
    pda_addStringTransition(p, qloop, qloop, "(", LEFT_PAREN, pda_dont_push);

    // ) token
    pda_addStringTransition(p, qloop, qloop, ")", RIGHT_PAREN, pda_dont_push);

    // completion transition
    pda_addStringTransition(p, qloop, qsuccess, "", pda_stack_empty, pda_dont_push);

    return p;
}

#endif

