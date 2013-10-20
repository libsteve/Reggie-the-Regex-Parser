#ifndef LIST_H
#define LIST_H

// a linked list node data structure
typedef struct list_node {
	struct list_node* next; // the next linked node
	struct list_node* prev; // the previous linked node
	void* value; // the pointer value held within the node
} *list_node;

// a linked list data structure
typedef struct list {
	struct list_node* connecting_node; 	// a node to connect the head and tail node in the linked list. 
										// the value will be a pointer to the list.
										// connecting_node->next is the head.
										// connecting_node->prev is the tail.
	int len; // the length of the list
} *list;

// create a linked list structure
list list_create();

// destroy a linked list structure
// use this to free all nodes and the list
void list_destroy(list l);

// push a pointer to the tail of the list
void list_push(list l, void* val);

// get the value from the tail of the list
void* list_peek(list l);

// remove the tail of the list and return the value
void* list_pop(list l);

// push a pointer to the head of the list
void list_rpush(list l, void* val);

// get the value from the head of the list
void* list_rpeek(list l);

// remove the head of the list and return the value
void* list_rpop(list l);

// enqueue a value to the head of the list
void list_enqueue(list l, void* value);

// dequeue a value from the tail of the list
void* list_dequeue(list l);

// the function type to use with list_foreach()
// takes a pointer value
typedef void (*foreach_func)(void* value);

// for each item in the list, execute the given function
void list_foreach(list l, foreach_func func);

// the function type to use with list_foreachWithState()
// takes a state pointer and a pointer value
typedef void (*foreachWithState_func)(void* state, void* value);

// for each item in the list, execute the given function with the given state
void list_foreachWithState(list l, foreachWithState_func func, void* state);

// get the length of the list
int list_len(list l);

// insert a value at the given index in the list
void list_insertAt(list l, int index, void* val);

// remove and return the value from the given index in the list
void* list_removeFrom(list l, int index);

// get the value from the index in the list
void* list_getFrom(list l, int index);

// get the index of the given pointer value from the list
// returns -1 if the pointer value was not found
int list_getIndex(list l, void* val);

// remove the given pointer value from the list
void list_removeValue(list l, void* val);

// the function type to use with list_find()
// takes a pointer to some sort of state (determined by you)
// and a pointer to the value at that part in the list.
// returns 1 if the value passed is the desired value,
// returns 0 otherwise.
typedef int (*find_func)(void* state, void* value);

// an index-value pair struct that packages the 
// index of a pointer value with the pointer value
typedef struct index_value {
	int index;
	void* value;
} index_value_pair;

// find the index and pointer value of the pointer value that satisfies
// the given find_func function that used the given state
index_value_pair list_find(list l, find_func func, void* state);

// take the two given lists, append the second to the end of the first,
// and return the composit of the two lists. 
// this destroys both given lists.
list list_merge(list a, list b);

// get the iterator value
#define VALUE(iterator) ((iterator)->value)

// iterate over each item in a list
// iterator - a symbol to use as an iterator
// list - the list to iterate over
#define FOREACH(iterator, list)	\
	for (	list_node (iterator) = (list)->connecting_node->next;	\
			(iterator)->value != (list);	\
			(iterator) = (iterator)->next	)

#endif