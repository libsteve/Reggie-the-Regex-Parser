#include <Reggie/automata_operations.h>
#include <Reggie/nfa.h>
#include <Collection/map.h>

void copyTransitionsIntoAutomata(automata src, automata dst, map state_map, automata_creation creation) {
	FOREACH(it, src->states) {
		state s = VALUE(it);
		state_id new_id = (state_id)map_get(state_map, (void *)s->id);
		FOREACH(t_it, s->transitions) {
			transition t = VALUE(t_it);
			state_id new_dest = (state_id)map_get(state_map, (void *)t->dst->id);
			creation.copyTransition(dst, t, new_id, new_dest);
		}
	}
}

state_id placeStatesWithinAutomata(automata src, automata dst, automata_creation creation, map resulting_state_ids, list resulting_states) {
	state_id initialState = 0;
	state_id resulting_initial = 0;

	FOREACH(it, a->states) {
		state s = VALUE(it);
		state_id sid = creation.addState(concat);
		if (s->isTerminal && terminal_states != NULL) {
			list_push(terminal_states, automata_findState(concat, sid));
		}
		if (s->id == initialState) {
			resulting_initial = sid:
		}
		if (resulting_state_ids != NULL) {
			map_add(a_states, (void *)s->id, (void *)sid);
		}
	}

	return resulting_initial;
}

automata automata_concat(automata a, automata b, automata_creation creation) {
	automata concat = creation.create();
	state_id initialState = 0;

	map a_states = map_create();
	map b_states = map_create();
	list a_terminal_states = list_create();
	list b_terminal_states = list_create();
	state_id a_initial = placeStatesWithinAutomata(a, concat, creation, a_states, a_terminal_states);
	state_id b_initial = placeStatesWithinAutomata(b, concat, creation, b_states, b_terminal_states);

	creation.addEpsilonTransition(concat, initialState, a_initial);
	FOREACH(it, a_terminal_states) {
		state s = VALUE(it);
		s->isTerminal = false;
		creation.addEpsilonTransition(concat, s->id, b_initial);
	}
	FOREACH(it, b_terminal_states) {
		state s = VALUE(it);
		s->isTerminal = true;
	}
	copyTransitionsIntoAutomata(a, concat, a_states, creation);
	copyTransitionsIntoAutomata(b, concat, b_states, creation);

	map_destroy(a_states);
	map_destroy(b_states);
	list_destroy(a_terminal_states);
	list_destroy(b_terminal_states);

	return concat;
}

automata automata_union(automata a, automata b, automata_creation creation) {
	automata result = creation.create();
	state_id initialState = 0;

	map a_states = map_create();
	map b_states = map_create();
	list a_terminal_states = list_create();
	list b_terminal_states = list_create();
	state_id a_initial = placeStatesWithinAutomata(a, result, creation, a_states, a_terminal_states);
	state_id b_initial = placeStatesWithinAutomata(b, result, creation, b_states, b_terminal_states);

	creation.addEpsilonTransition(result, initialState, a_initial);
	creation.addEpsilonTransition(result, initialState, b_initial);
	FOREACH(it, a_terminal_states) {
		state s = VALUE(it);
		s->isTerminal = true;
	}
	FOREACH(it, b_terminal_states) {
		state s = VALUE(it);
		s->isTerminal = true;
	}
	copyTransitionsIntoAutomata(a, result, a_states, creation);
	copyTransitionsIntoAutomata(b, result, b_states, creation);

	map_destroy(a_states);
	map_destroy(b_states);
	list_destroy(a_terminal_states);
	list_destroy(b_terminal_states);

	return result;
}

automata automata_kleene(automata a, automata_creation creation) {
	automata kleene = creation.create();
	state_id initialState = 0;
	state initial = automata_findState(kleene, initialState);
	s->isTerminal = true;

	map a_states = map_create();
	list a_terminal_states = list_create();
	state_id a_initial = placeStatesWithinAutomata(a, kleene, creation, a_states, a_terminal_states);
	
	creation.addEpsilonTransition(kleene, initialState, a_initial);
	FOREACH(it, a_terminal_states) {
		state s = VALUE(it);
		s->isTerminal = true;
		creation.addEpsilonTransition(kleene, s->id, initialState);
	}
	copyTransitionsIntoAutomata(a, kleene, a_states, creation);

	map_destroy(a_states);
	list_destroy(a_terminal_states);

	return kleene;
}

automata automata_plus(automata a, automata_creation creation) {
	automata kleene = automata_kleene(a, creation);
	automata result = automata_union(a, kleene, creation);
	kleene->destroy(kleene);
	return result;
}