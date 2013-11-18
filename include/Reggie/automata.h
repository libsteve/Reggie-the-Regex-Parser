#ifndef REGGIE_AUTOMATA_H
#define REGGIE_AUTOMATA_H

#include <Collection/list.h>
#include <stddef.h>
#include <stdbool.h>

#ifndef container_of
#define container_of(ptr, container_struct, containing_member) \
	((container_struct *) ((char *)(ptr) - (char *)offsetof(container_struct, containing_member)))
#endif


struct transition;
struct state;
struct automata;


typedef struct transition 	*transition;
typedef struct state 		*state;
typedef struct automata 	*automata;

typedef unsigned int 		state_id;
typedef unsigned int 		transition_id;

typedef struct evalstream {
	struct evalstream *(*fastforward)(struct evalstream *stream, int amount);
	struct evalstream *(*rewind)(struct evalstream *stream, int amount);
	bool (*closed)(struct evalstream *stream);
} evalstream;

// returns -1 if failure, otherwise returns the length of a success
typedef int (*transition_func)(const struct automata *a, const struct transition *t, const evalstream *input);

typedef void (*state_destroy)(struct state *s);
typedef void (*transition_destroy)(struct transition *t);
typedef void (*automata_destroy)(struct automata *a);


struct transition {
	transition_id id;
	struct state *src;
	struct state *dst;
	transition_func func;
	transition_destroy destroy;
};

struct state {
	state_id id;
	bool isTerminal;
	list transitions;
	state_destroy destroy;
};

struct automata {
	list states;
	list transitions;
	state_id 		next_state_id;
	transition_id 	next_transition_id;
	automata_destroy destroy;
};


transition 	transition_initialize(transition t, state src, state dest, transition_func func, transition_destroy destroy);
void 		transition_uninitialize(transition t);

state 		state_initialize(state s, bool isTerminal, state_destroy destroy);
void 		state_uninitialize(state s);

automata 	automata_initialize(automata a, automata_destroy destroy);
void 		automata_uninitialize(automata a);

state_id 		automata_addState(automata a, state s);
transition_id 	automata_addTransition(automata a, transition t);

void 		automata_removeState(automata a, state_id sid);
void 		automata_removeTransition(automata a, transition_id tid);

state 		automata_findState(automata a, state_id sid);
transition 	automata_findTransition(automata a, transition_id tid);

#endif
