#include <Reggie/automata.h>

transition transition_initialize(transition t, state src, state dst, transition_apply apply, transition_revoke revoke, transition_destroy destroy) {
	t->id = 0;
	t->src = src;
	t->dst = dst;
	t->apply = apply;
	t->revoke = revoke;
	t->destroy = destroy;
	return t;
}

void transition_uninitialize(transition t) {
	t->src = NULL;
	t->dst = NULL;
}

state state_initialize(state s, bool isTerminal, state_destroy destroy) {
	s->id = 0;
	s->transitions = list_create();
	s->isTerminal = isTerminal;
	s->destroy = destroy;
	return s;
}

void state_uninitialize(state s) {
	list_destroy(s->transitions);
	s->transitions = NULL;
}

automata automata_initialize(automata a, automata_destroy destroy) {
	a->states = list_create();
	a->transitions = list_create();
	a->next_state_id = 0;
	a->next_transition_id = 0;
	a->destroy = destroy;
	return a;
}

void automata_uninitialize(automata a) {
	FOREACH(it, a->transitions) {
		transition t = VALUE(it);
		t->destroy(t);
	}
	FOREACH(it, a->states) {
		state s = VALUE(it);
		s->destroy(s);
	}
	list_destroy(a->states);
	list_destroy(a->transitions);
	a->states = NULL;
	a->transitions = NULL;
}

state_id automata_addState(automata a, state s) {
	s->id = a->next_state_id;
	a->next_state_id += 1;
	list_push(a->states, s);
	return s->id;
}

transition_id automata_addTransition(automata a, transition t) {
	state s = automata_findState(a, t->src->id);
	if (s != NULL) {
		t->id = a->next_transition_id;
		a->next_transition_id += 1;
		list_push(a->transitions, t);
		list_push(s->transitions, t);
	}
	return t->id;
}

void automata_removeState(automata a, state_id sid) {
	state s = automata_findState(a, sid);
	FOREACH(it, a->transitions) {
		transition t = VALUE(it);
		if (t->src->id == sid || t->dst->id == sid) {
			automata_removeTransition(a, t->id);
		}
	}
	list_removeValue(a->states, s);
	s->destroy(s);
}

void automata_removeTransition(automata a, transition_id tid) {
	FOREACH(sit, a->states) {
		state s = VALUE(sit);
		list to_destroy = list_create();
		FOREACH(tit, s->transitions) {
			transition t = VALUE(tit);
			if (t->id == tid) {
				list_push(to_destroy, t);
			}
		}
		FOREACH(tit, to_destroy) {
			list_removeValue(s->transitions, VALUE(tit));
		}
		list_destroy(to_destroy);
	}
	transition t = automata_findTransition(a, tid);
	if (t) {
		list_removeValue(a->transitions, t);
		t->destroy(t);
	}
}

state automata_findState(automata a, state_id sid) {
	FOREACH(it, a->states) {
		state s = VALUE(it);
		if (s->id == sid) {
			return s;
		}
	}
	return NULL;
}

transition automata_findTransition(automata a, transition_id tid) {
	FOREACH(it, a->transitions) {
		transition t = VALUE(it);
		if (t->id == tid) {
			return t;
		}
	}
	return NULL;
}
