#include "list_sort.h"

list sortOne(list l, sort_fn sorter, int s, int e);
list sortTwo(list l, sort_fn sorter, int s, int e);
list sortThree(list l, sort_fn sorter, int s, int e);
list sortPivot(list l, sort_fn sorter, int s, int e, int p);
list sortFrom(list l, sort_fn sorter, int s, int e);

list sortOne(list l, sort_fn sorter, int s, int e) {
	list r = list_create();
	list_push(r, list_getFrom(l, s));
	return r;
}

list sortTwo(list l, sort_fn sorter, int s, int e) {
	void *a = list_getFrom(l, s);
	void *b = list_getFrom(l, s+1);
	list r = list_create();
	if (sorter(a, b)) {
		list_push(r, a);
		list_push(r, b);
	} else {
		list_push(r, b);
		list_push(r, a);
	}
	return r;
}

list sortThree(list l, sort_fn sorter, int s, int e) {
	list r = sortTwo(l, sorter, s, e-1);
	void *b = list_getFrom(l, s+1);
	void *c = list_getFrom(l, s+2);
	if (sorter(b, c)) {
		list_push(r, c);
	} else {
		list_insertAt(r, 1, c);
	}
	return r;
}

list sortPivot(list l, sort_fn sorter, int s, int e, int p) {
	list left = list_create();
	list right = list_create();
	void *pivot = list_getFrom(l, p);
	int i = 0;
	FOREACH(it, l) {
		void *val = VALUE(it);
		if (i != p) {
			if (sorter(val, pivot)) {
				list_push(left, val);
			} else {
				list_push(right, val);
			}
		}
		i++;
	}
	list r = sortFrom(left, sorter, 0, list_len(left));
	list_push(r, pivot);
	r = list_merge(r, sortFrom(right, sorter, 0, list_len(right)));
	list_destroy(left);
	list_destroy(right);
	return r;
}

list sortFrom(list l, sort_fn sorter, int s, int e) {
	switch (e - s) {
	case 0:
		return list_create();
	case 1: 
		return sortOne(l, sorter, s, e);
	case 2:
		return sortTwo(l, sorter, s, e);
	case 3:
		return sortThree(l, sorter, s, e);
	default:
		if ((e-s) % 2 == 0)
			return sortPivot(l, sorter, s, e, (e-s)/2);
		else
			return sortPivot(l, sorter, s, e, (e-s+1)/2);
	}
}

list list_sort(list l, sort_fn sorter) {
	return sortFrom(l, sorter, 0, list_len(l));
}