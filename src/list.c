#include <stdlib.h>
#include "list.h"

list list_create() {
	// create the list
	list l = calloc(1, sizeof(struct list));

	// create the connecting_node
	l->connecting_node = calloc(1, sizeof(struct list_node));
	l->connecting_node->value = l;
	l->connecting_node->next = l->connecting_node;
	l->connecting_node->prev = l->connecting_node;
	l->head = l->connecting_node;
	l->tail = l->connecting_node;

	return l;
}

void list_destroy(list l) {
	list_node n = l->head;
	while (n->value != l) {
		list_node next = n->next;
		free(n);
		n = next;
	}
	free(n);
	free(l);
}

void list_push(list l, void* val) {
	list_node n = calloc(1, sizeof(struct list_node));

	n->prev = l->tail;
	l->tail->next = n;
	n->next = l->connecting_node;
	l->connecting_node->prev = n;
	l->tail = n;
}

void* list_peek(list l) {
	if (l->tail->value == l)
		return 0;
	return l->tail->value;
}

void* list_pop(list l) {
	if (l->tail->value == l)
		return 0;
	
	list_node tail = l->tail->prev;
	list_node n = l->tail;

	tail->next = l->connecting_node;
	l->connecting_node->prev = tail;

	void* value = n->value;
	free(n);
	return value;
}

void list_foreach(list l, foreach_func func) {
	list_node n = l->head;
	while (n->value != l) {
		list_node next = n-> next;
		func(n->value);
		n = next;
	}
}

void list_foreachWithState(list l, foreachWithState_func func, void* state) {
	list_node n = l->head;
	while (n->value != l) {
		list_node next = n-> next;
		func(state, n->value);
		n = next;
	}
}

int list_len(list l) {
	list_node n = l->head;
	int result = 0;
	while (n->value != l) {
		result++;
		n = n->next;
	}
	return result;
}

void list_insertAt(list l, int index, void* val) {
	if (index >= list_len(l)) 
		list_push(l, val);
	else if (index < 0)
		list_insertAt(l, 0, val);
	else {
		int curr_index = 0;
		list_node n = l->head;
		while (curr_index < index) {
			curr_index++;
			n = n->next;
		}
		list_node next = n;
		list_node prev = n->prev;
		n = calloc(1, sizeof(struct list_node));
		n->next = next;
		next->prev = n;
		n->prev = prev;
		prev->next = n;
		n->value = val;
	}
}

void* list_removeFrom(list l, int index) {
	int len = list_len(l);
	if (len == 0) return 0;

	if (index >= len-1) 
		return list_pop(l);
	int curr_index = 0;
	list_node n = l->head;
	while (curr_index < index) {
		curr_index++;
		n = n->next;
	}
	void* val = n->value;
	free(n);
	return val;
}

void* list_getFrom(list l, int index) {
	int len = list_len(l);
	if (len == 0) return 0;

	if (index >= len-1) 
		return list_peek(l);
	int curr_index = 0;
	list_node n = l->head;
	while (curr_index < index) {
		curr_index++;
		n = n->next;
	}
	return n->value;
}

int list_getIndex(list l, void* val) {
	list_node n = l->head;
	int index = 0;
	while (n->value != l) {
		if (n->value == val)
			return index;
		index++;
		n = n->next;
	}
	return -1;
}

void list_removeValue(list l, void* val) {
	int index = list_getIndex(l, val);
	if (index != -1) {
		list_removeFrom(l, index);
	}
}

index_value_pair list_find(list l, find_func func, void* state) {
	list_node n = l->head;
	int index = 0;
	while (n->value != l) {
		list_node next = n-> next;
		func(state, n->value);
		n = next;
		index++;
	}
	if (n->value == l)
		return (index_value_pair){-1, 0};
	return (index_value_pair){index, n->value};
}