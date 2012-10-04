#ifndef LIST_H
#define LIST_H

typedef struct list_node {
	struct list_node* next;
	struct list_node* prev;
	void* value;
} *list_node;

typedef struct list {
	struct list_node* head;
	struct list_node* tail;
	struct list_node* connecting_node;
} *list;

list list_create();

void list_destroy(list l);

void list_push(list l, void* val);

void* list_peek(list l);

void* list_pop(list l);

typedef void (*foreach_func)(void* value);

int list_len(list l);

void list_insertAt(list l, int index, void* val);

void* list_removeFrom(list l, int index);

void* list_getFrom(list l, int index);

#endif