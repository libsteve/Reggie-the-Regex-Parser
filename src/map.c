#include "map.h"
#include "strings.h"
#include <stdlib.h>

// create a new map_entry instance
map_entry map_entry_create() {
	map_entry e = calloc(1, sizeof(struct map_entry));
	return e;
}

// destroy a map_entry instance
void map_entry_destroy(map_entry entry) {
	free(entry);
}

int map_default_key_comp(void *a, void *b) {
	return a == b;
}

// create a new map instance
map map_create() {
	map m = calloc(1, sizeof(struct map));
	m->map = list_create();
	m->key_compare = &map_default_key_comp;
	return m;
}

// create a new map instance that compares keys as strings
map map_createStringMap() {
	map m = map_create();
	m->key_compare = (key_eq)&string_equals;
	return m;
}

// destroy a map instance
// destroys all map_entry instances within
// does not destroy any given keys or values
void map_destroy(map m) {
	FOREACH(it, m->map) {
		map_entry_destroy(VALUE(it));
	}
	list_destroy(m->map);
	free(m);
}

// set the key comparison function for the map
void map_setKeyCompare(map m, key_eq function) {
	m->key_compare = function;
}

map_entry map_findEntry(map m, void *key) {
	key_eq fn = m->key_compare;
	FOREACH_ENTRY(ent, m) {
		if (fn(key, ENTRY_KEY(ent))) {
			return VALUE(ent);
		}
	}
	return 0;
}

// set the value for the given key in the map
void map_set(map m, void *key, void *value) {
	map_entry entry = map_findEntry(m, key);
	if (entry == 0) {
		entry = map_entry_create();
		entry->key = key;
		list_push(m->map, entry);
	}
	entry->value = value;
}

// delete the key and associated value from the map
void map_delete(map m, void *key) {
	void *entry = map_findEntry(m, key);
	if (entry != 0) {
		list_removeValue(m->map, entry);
	}
}

// get the value from the map for the given key
void *map_get(map m, void *key) {
	map_entry entry = map_findEntry(m, key);
	if (entry == 0)
		return 0;
	return entry->value;
}

// return 1 if there is an entry for the given key, otherwise return 0
int map_has(map m, void *key) {
	void *entry = map_findEntry(m, key);
	if (entry == 0)
		return 0;
	return 1;
}

// get the amount of key-value pairs within the map
int map_size(map m) {
	return list_len(m->map);
}