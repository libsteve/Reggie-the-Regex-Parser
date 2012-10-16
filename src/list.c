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

// private function
// insert the given node a before the given node b
// the list is circularly and doubbly linked, so we have access to the node before b
void __node_insertBefore(list_node a, list_node b) {
	b->prev->next = a;
	a->prev = b->prev;
	a->next = b;
	b->prev = a;
}

// private function
// insert the given node a after the given node b
// the list is circularly and doubbly linked, so we have access to the node after b
void __node_insertAfter(list_node a, list_node b) {
	a->next = b->next;
	b->next->prev = a;
	b->next = a;
	a->prev = b;
}

// private function
// remove the node before the given node
list_node __node_removeBefore(list_node a) {
	list_node removed = a->prev;

	a->prev->prev->next = a;
	a->prev = a->prev->prev;

	return removed;
}

// private function
// remove the node after the given node
list_node __node_removeAfter(list_node a) {
	list_node removed = a->next;

	a->next->next->prev = a;
	a->next = a->next->next;

	return removed;
}

void list_push(list l, void* val) {
	list_node n = calloc(1, sizeof(struct list_node));

	__node_insertAfter(n, l->tail);
	l->tail = n;
	l->len += 1;

	if (l->len == 0) l->head = n;
}

void list_rpush(list l, void* val) {
	list_node n = calloc(1, sizeof(struct list_node));

	__node_insertBefore(n, l->head);
	l->head = n;
	l->len += 1;

	if (l->len == 0) l->tail = n;
}

void* list_peek(list l) {
	if (l->tail->value == l)
		return 0;
	return l->tail->value;
}

void* list_rpeek(list l) {
	if (l->head->value == l)
		return 0;
	return l->head->value;
}

void* list_pop(list l) {
	if (l->tail->value == l)
		return 0;
	
	list_node n = __node_removeAfter(l->tail->prev);
	l->tail = n->prev;
	l->len -= 1;

	if (l->len <= 1)
		l->head = l->tail;

	void* value = n->value;
	free(n);
	return value;
}

void* list_rpop(list l) {
	if (l->head->value == l)
		return 0;
	
	list_node n = __node_removeBefore(l->head->next);
	l->head = n->next;
	l->len -= 1;

	if (l->len <= 1)
		l->tail = l->head;

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
	return l->len;
}

// private function
// get the Nth node from the list
// if n >= the length -1, return the tail
// if n <= 0, return the head
list_node __list_getNthNode(list l, int n) {
	if (l->len != 0) {
		if (n <= 0)
			return l->head;
		else if (n >= l->len - 1)
			return l->tail;
		else {
			int curr_index = 0;
			list_node curr = l->head;
			while (curr_index < n) {
				curr_index++;
				curr = curr->next;
			}
			return curr;
		}
	} else {
		return 0;
	}
}

void list_insertAt(list l, int index, void* val) {
	list_node i = __list_getNthNode(l, index);
	if (i == l->head)
		list_rpush(l, val);
	else if (i == l->tail)
		list_push(l, val);
	else {
		list_node n = calloc(1, sizeof(struct list_node));
		__node_insertBefore(n, i);
		n->value = val;
		l->len += 1;
	}
}

void* list_removeFrom(list l, int index) {
	if (l->len == 0) return 0;

	list_node n = __list_getNthNode(l, index);
	if (n == l->head)
		return list_rpop(l);
	else if (n == l->tail)
		return list_pop(l);
	else {
		__node_removeAfter(n->prev);
		void* val = n->value;
		free(n);
		l->len -= 1;
		return val;
	}
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