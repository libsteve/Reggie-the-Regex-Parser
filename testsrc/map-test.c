#include "tester.h"
#include "Collection/map.h"

result test_() {
	int passed = 0;
	char* description = "";

	

	return (result){passed, description};
}

////
// test map_set()

result test_map_set_1() {
	int passed = 0;
	char* description = "map_set() : CASE 1 element";

	map m = map_create();
	map_set(m, "abc", "123");
	passed = is_equal(1, map_size(m));
	map_destroy(m);

	return (result){passed, description};
}

result test_map_set_2() {
	int passed = 0;
	char* description = "map_set() : CASE 2 element";

	map m = map_create();
	map_set(m, "abc", "123");
	map_set(m, "def", "456");
	passed = is_equal(2, map_size(m));
	map_destroy(m);

	return (result){passed, description};
}

result test_map_set_3() {
	int passed = 0;
	char* description = "map_set() : CASE 2 element, same key";

	char *key = "abc";

	map m = map_create();
	map_set(m, key, "123");
	map_set(m, key, "456");
	passed = is_equal(1, map_size(m));
	map_destroy(m);

	return (result){passed, description};
}

////
// test map_delete()

result test_map_delete_1() {
	int passed = 0;
	char* description = "map_delete() : CASE delete only element";

	char *key = "abc";

	map m = map_create();
	map_set(m, key, "123");
	map_delete(m, key);
	passed = is_equal(0, map_size(m));
	map_destroy(m);

	return (result){passed, description};
}

result test_map_delete_2() {
	int passed = 0;
	char* description = "map_delete() : CASE 2 elements, delete 1";

	char *key = "abc";

	map m = map_create();
	map_set(m, key, "123");
	map_set(m, "def", "456");
	map_delete(m, key);
	passed = is_equal(1, map_size(m));
	map_destroy(m);

	return (result){passed, description};
}

////
// test map_get()

result test_map_get_1() {
	int passed = 0;
	char* description = "map_get() : CASE get only element";

	char *key = "abc";
	int a = 5;

	map m = map_create();
	map_set(m, key, &a);
	int *b = map_get(m, key);
	passed = is_equal(&a, b);
	map_destroy(m);

	return (result){passed, description};
}

result test_map_get_2() {
	int passed = 0;
	char* description = "map_get() : CASE get 2nd element";

	char *key = "abc";
	int a = 5;

	map m = map_create();
	map_set(m, "def", "123");
	map_set(m, key, &a);
	int *b = map_get(m, key);
	passed = is_equal(&a, b);
	map_destroy(m);

	return (result){passed, description};
}

////
// test map_has()

result test_map_has_1() {
	int passed = 0;
	char* description = "map_has() : CASE no elements";

	map m = map_create();
	passed = is_false(map_has(m, "key"));
	map_destroy(m);

	return (result){passed, description};
}

result test_map_has_2() {
	int passed = 0;
	char* description = "map_has() : CASE no elements";

	char *key = "abc";

	map m = map_create();
	map_set(m, key, "123");
	passed = is_true(map_has(m, key));
	map_destroy(m);

	return (result){passed, description};
}

////
// test FOREACH_ENTRY()

result test_foreach_1() {
	int passed = 0;
	char* description = "FOREACH_ENTRY() : CASE no elements";

	map m = map_create();
	passed = 1;
	FOREACH_ENTRY(it, m) {
		passed = 0;
	}
	map_destroy(m);

	return (result){passed, description};
}

result test_foreach_2() {
	int passed = 0;
	char* description = "FOREACH_ENTRY() : CASE 1 element";

	map m = map_create();
	map_set(m, "abc", "123");
	passed = 0;
	int count = 0;
	FOREACH_ENTRY(it, m) {
		if (count != 0) passed = 0;
		else passed = 1;
		count++;
	}
	map_destroy(m);

	return (result){passed, description};
}

result test_foreach_3() {
	int passed = 0;
	char* description = "FOREACH_ENTRY() : CASE 2 elements";

	char *k1 = "abc";
	char *k2 = "def";

	char *v1 = "123";
	char *v2 = "456";

	map m = map_create();
	map_set(m, k1, v1);
	map_set(m, k2, v2);
	passed = 0;
	int count = 0;
	FOREACH_ENTRY(it, m) {
		if (count == 0) {
			passed = is_equal(k1, ENTRY_KEY(it)) &&
					 is_equal(v1, ENTRY_VALUE(it));
		} else if (count == 1) {
			passed = passed && 
					 is_equal(k2, ENTRY_KEY(it)) &&
					 is_equal(v2, ENTRY_VALUE(it));
		} else {
			passed = 0;
		}
		count++;
	}
	map_destroy(m);

	return (result){passed, description};
}

////
// main function and test list definition

static tests TESTS = {
	test_map_set_1,
	test_map_set_2,
	test_map_set_3,

	test_map_delete_1,
	test_map_delete_2,

	test_map_get_1,
	test_map_get_2,

	test_map_has_1,
	test_map_has_2,

	test_foreach_1,
	test_foreach_2,
	test_foreach_3,

	0
};

int main(int argc, char** argv) {
	run_tests(TESTS);
	return 0;
}