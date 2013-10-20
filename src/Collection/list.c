#include <stdlib.h>
#include "Collection/list.h"

#define HEAD connecting_node->next
#define TAIL connecting_node->prev

list list_create() {
	// create the list
	list l = calloc(1, sizeof(struct list));

	// create the connecting_node
	l->connecting_node = calloc(1, sizeof(struct list_node));
	l->connecting_node->value = l;
	l->connecting_node->next = l->connecting_node;
	l->connecting_node->prev = l->connecting_node;
	l->len = 0;

	return l;
}

void list_destroy(list l) {
	list_node n = l->HEAD;
	while (n->value != l) {
		list_node next = n->next;
		free(n);
		n = next;
	}
	free(n);
	free(l);
}

// private
// put n between a and b
// a -> b  :=>  a -> n -> b
void __node_insertBetween(list_node n, list_node a, list_node b) {
	n->prev = a;
	n->next = b;

	a->next = n;
	b->prev = n;
}

// private
// remove n
// n must have an n->prev and n->next
// x -> n -> y  :=>  x -> y
void __node_remove(list_node n) {
	list_node prev = n->prev;
	list_node next = n->next;

	prev->next = next;
	next->prev = prev;
}

void list_push(list l, void* value) {
	list_node n = calloc(1, sizeof(struct list_node));
	__node_insertBetween(n, l->TAIL, l->TAIL->next);
	n->value = value;
	l->len += 1;
}

void* list_peek(list l) {
	if (l->TAIL->value == l) return 0;
	return l->TAIL->value;
}

void* list_pop(list l) {
	if (l->TAIL->value == l)
		return 0;

	list_node n = l->TAIL;
	__node_remove(n);
	l->len -= 1;

	void* value = n->value;
	free(n);
	return value;
}

void list_rpush(list l, void* value) {
	list_node n = calloc(1, sizeof(struct list_node));
	__node_insertBetween(n, l->HEAD->prev, l->HEAD);
	n->value = value;
	l->len += 1;
}

void* list_rpeek(list l) {
	if (l->HEAD->value == l) return 0;
	return l->HEAD->value;
}

void* list_rpop(list l) {
	if (l->HEAD->value == l)
		return 0;

	list_node n = l->HEAD;
	__node_remove(n);
	l->len -= 1;
	
	void* value = n->value;
	free(n);
	return value;
}

void list_enqueue(list l, void* value) {
	list_rpush(l, value);
}

void* list_dequeue(list l) {
	return list_pop(l);
}

void list_foreach(list l, foreach_func func) {
	FOREACH(it, l) {
		func(VALUE(it));
	}
}

void list_foreachWithState(list l, foreachWithState_func func, void* state) {
	FOREACH(it, l) {
		func(state, VALUE(it));
	}
}

int list_len(list l) {
	return l->len;
}

// private function
// get the Nth node from the list
// if n >= the length -1, return the tail
// if n <= 0, return the head
list_node __list_getNthNode(list l, int n) {
	if (l->len != 0) {
		if (n <= 0)
			return l->HEAD;
		else if (n >= l->len - 1)
			return l->TAIL;
		else {
			int curr_index = 0;
			list_node curr = l->HEAD;
			while (curr_index < n) {
				curr_index++;
				curr = curr->next;
			}
			return curr;
		}
	} else {
		return l->HEAD;
	}
}

void list_insertAt(list l, int index, void* val) {
	if (l->len <= index)
		list_push(l, val);
	else if (index <= 0)
		list_rpush(l, val);
	else {
		list_node i = __list_getNthNode(l, index);
		list_node n = calloc(1, sizeof(struct list_node));
		__node_insertBetween(n, i->prev, i);
		n->value = val;
		l->len += 1;
	}
}

void* list_removeFrom(list l, int index) {
	if (l->len == 0) return 0;

	list_node n = __list_getNthNode(l, index);
	if (n == l->HEAD)
		return list_rpop(l);
	else if (n == l->TAIL)
		return list_pop(l);
	else {
		__node_remove(n);
		void* val = n->value;
		free(n);
		l->len -= 1;
		return val;
	}
}

void* list_getFrom(list l, int index) {
	list_node n = __list_getNthNode(l, index);
	if (n->value == l) return 0;
	return n->value;
}

int list_getIndex(list l, void* val) {
	int index = 0;
	FOREACH(it, l) {
		if(VALUE(it) == val)
			return index;
		index++;
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
	int index = 0;
	FOREACH(it, l) {
		if (func(state, VALUE(it)))
			return (index_value_pair){index, VALUE(it)};
		index++;
	}
	return (index_value_pair){-1, 0};
}

list list_merge(list a, list b) {
	list l = list_create();

	if (a->len > 0 && b->len > 0) /* if neither a nor b are empty */ {

		// set up the length
		l->len = a->len + b->len;

		// set up the head of the new list with that of a
		l->HEAD = a->HEAD;
		l->HEAD->prev = l->connecting_node;
		l->connecting_node->next = l->HEAD;

		// connect the tail of a to the head of b
		a->TAIL->next = b->HEAD;
		b->HEAD->prev = a->TAIL;

		// connect a->connecting_node to itself
		a->connecting_node->next = a->connecting_node;
		a->connecting_node->prev = a->connecting_node;

		// set up the tail of the new list with that of b
		l->TAIL = b->TAIL;
		l->TAIL->next = l->connecting_node;
		l->connecting_node->prev = l->TAIL;

		// connect b->connecting_node to itself
		b->connecting_node->next = b->connecting_node;
		b->connecting_node->prev = b->connecting_node;

	} else if (a->len > 0 && b->len <= 0) /* if b is empty */ {
		// this ensures a and b are freed
		// and their pointers are rendered invalid
		
		// set up the length
		l->len = a->len;

		// set up the head of the new list with that of a
		l->HEAD = a->HEAD;
		l->HEAD->prev = l->connecting_node;
		l->connecting_node->next = l->HEAD;

		// set up the tail of the new list with that of a
		l->TAIL = a->TAIL;
		l->TAIL->next = l->connecting_node;
		l->connecting_node->prev = l->TAIL;

		// connect a->connecting_node to itself
		a->connecting_node->next = a->connecting_node;
		a->connecting_node->prev = a->connecting_node;

	} else if (a->len <= 0 && b->len > 0) /* if a is empty */ {
		// this ensures a and b are freed
		// and their pointers are rendered invalid

		// set up the length
		l->len = b->len;

		// set up the head of the new list with that of b
		l->HEAD = b->HEAD;
		l->HEAD->prev = l->connecting_node;
		l->connecting_node->next = l->HEAD;

		// set up the tail of the new list with that of b
		l->TAIL = b->TAIL;
		l->TAIL->next = l->connecting_node;
		l->connecting_node->prev = l->TAIL;

		// connect b->connecting_node to itself
		b->connecting_node->next = b->connecting_node;
		b->connecting_node->prev = b->connecting_node;

	} /* return an empty list if both a and b are empty */

	list_destroy(a);
	list_destroy(b);
	return l;
}

#undef HEAD
#undef TAIL