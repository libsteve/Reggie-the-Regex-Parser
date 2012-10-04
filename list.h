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
	struct list_node* head; // the head node in the linked list
	struct list_node* tail; // the tail node in the linked list
	struct list_node* connecting_node; // a node to connect the head and tail node in the linked list. the value will be a pointer to the list
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

// the function type to use with list_foreach()
// takes a pointer value
typedef void (*foreach_func)(void* value);

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

#endif