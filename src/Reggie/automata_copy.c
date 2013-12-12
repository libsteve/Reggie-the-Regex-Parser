#include <Reggie/automata_copy.h>
#include <Collection/map.h>

automata automata_copy(automata a, automata_copy_creation acc) {
	automata copy = acc.create();

	map state_map = map_create();

	FOREACH(it, a->states) {
		state s = VALUE(it);
		state_id old = s->id;
		state_id new = acc.copyState(copy, s);
		map_set(state_map, (void *)old, (void *)new);
	}

	FOREACH(it, a->transitions) {
		transition t = VALUE(it);
		acc.copyTransition(copy, t, (state_id)map_get(state_map, (void *)t->src->id), (state_id)map_get(state_map, (void *)t->dst->id));
	}

	map_destroy(state_map);

	return copy;
}
