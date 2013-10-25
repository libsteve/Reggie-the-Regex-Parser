#ifndef REGGIE_AUTOMATA_H
#define REGGIE_AUTOMATA_H

#include <Collection/list.h>
#include <stdtypes.h>
#include <stdbool.h>


#define offsetof(MEMBER, STRUCT) ((size_t) &(((STRUCT *)0)->MEMBER))
#define container_of(PTR, CONTAINER_STRUCT, CONTAINING_MEMBER) \
	((CONTAINER_STRUCT *) ((char *)(PTR) - (char *)offsetof(CONTAINING_MEMBER, CONTAINER_STRUCT)))


struct transition;
struct state;
struct automata;


typedef struct transition 	*transition;
typedef struct state 		*state;
typedef struct automata 	*automata;

typedef unsigned int 		state_id;
typedef unsigned int 		transition_id;

// returns -1 if failure, otherwise returns the length of a success
typedef int (*transition_func)(const struct automata *a, const struct transition *t, const char *input);

typedef void (*state_uninitialize)(struct state *s);
typedef void (*transition_uninitialize)(struct transition *t);


struct transition {
	transition_id id;
	struct state *src;
	struct state *dst;
	transition_func func;
	transition_uninitialize uninitialize;
};

struct state {
	state_id id;
	bool isTerminal;
	list transitions;
	state_uninitialize uninitialize;
};

struct automata {
	list states;
	list transitions;
	state_id 		next_state_id;
	transition_id 	next_transition_id;
};


transition 	transition_initialize(transition t, state src, state dest, transition_func func, transition_uninitialize uninitialize);
void 		transition_uninitialize(transition t);

state 		state_initialize(state s, bool isTerminal, state_uninitialize uninitialize);
void 		state_uninitialize(state s);

automata 	automata_initialize(automata a);
void 		automata_uninitialize(automata a);

state_id 		automata_addState(automata a, state s);
transition_id 	automata_addTransition(autamata a, transition t);

#endif
