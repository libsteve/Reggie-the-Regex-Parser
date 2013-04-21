#ifndef MAP_H
#define MAP_H

#include "list.h"

// function tyoes for key comparitor that determine whether
// the two given key values are equal to eachother.
// returns 1 if equal, 0 otherwise
typedef int (*key_eq)(void *a, void *b);

// a structure to represent a map entry with a key and a value
typedef struct map_entry {
	void *key;	// the key value for the entry
	void *value;	// the value for the entry
} *map_entry;

// create a new map_entry instance
map_entry map_entry_create();

// destroy a map_entry instance
void map_entry_destroy(map_entry entry);

// a structure to represent a map of keys to values
typedef struct map {
	list map;	// a list of all the map entries
	key_eq key_compare;	// a function to determine if two keys are the same
						// the default behavior is to compare pointer values
} *map;

// create a new map instance
map map_create();

// create a new map instance that compares keys as strings
map map_createStringMap();

// destroy a map instance
// destroys all map_entry instances within
// does not destroy any given keys or values
void map_destroy(map m);

// set the key comparison function for the map
void map_setKeyCompare(map m, key_eq function);

// set the value for the given key in the map
void map_set(map m, void *key, void *value);

// delete the key and associated value from the map
void map_delete(map m, void *key);

// get the value from the map for the given key
void *map_get(map m, void *key);

// return 1 if there is an entry for the given key, otherwise return 0
int map_has(map m, void *key);

// get the amount of key-value pairs within the map
int map_size(map m);

// get the entry iterator's key value
#define ENTRY_KEY(iterator)	((iterator)->value->key)

// get the entry itarator's value value
#define ENTRY_VALUE(iterator)	((iterator)->value->value)

// loop over every entry within a map
// iterator - a symbol used as an iterator ove map entries
// map - the map to iterate over
#define FOREACH_ENTRY(iterator, map)	\
	for (	list_node (iterator) = (map->map)->connecting_node->next;	\
			(iterator)->value != (map->map);	\
			(iterator) = (iterator)->next	)

#endif