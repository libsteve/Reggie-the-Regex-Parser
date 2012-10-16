#include "strings-test.h"

////
// test int string_length(char*)

result test_string_length_EmptyString() {
	int passed = 0;
	char* description = "string_length(char*) : EmptyString CASE";

	passed = is_equal(string_length(""),0);

	return (result){passed, description};
}

result test_string_length_NULL() {
	int passed = 0;
	char* description = "string_length(char*) : NULL CASE";

	passed = is_equal(string_length(0), -1);

	return (result){passed, description};
}

result test_string_length_Len1() {
	int passed = 0;
	char* description = "string_length(char*) : Length==1 CASE";

	passed = is_equal(string_length("1"), 1);

	return (result){passed, description};
}

result test_string_length_Len5() {
	int passed = 0;
	char* description = "string_length(char*) : Length==5 CASE";

	passed = is_equal(string_length("12345"), 5);

	return (result){passed, description};
}

////
// Test int string_equals(char*, char*)

result test_string_equals_EmptyStrings() {
	int passed = 0;
	char* description = "string_equals(char*, char*) : 2EmptyString CASE";

	passed = is_true(string_equals("", ""));

	return (result){passed, description};
}

result test_string_equals_EmptyStringNULL() {
	int passed = 0;
	char* description = "string_equals(char*, char*) : EmptyStringNULL CASE";

	passed = is_false(string_equals("", 0));

	return (result){passed, description};
}

result test_string_equals_NULL() {
	int passed = 0;
	char* description = "string_equals(char*, char*) : NULL CASE";

	passed = is_false(string_equals(0, 0));

	return (result){passed, description};
}

result test_string_equals_StringWithEmpty() {
	int passed = 0;
	char* description = "string_equals(char*, char*) : StringWithEmpty CASE";

	passed = is_false(string_equals("abc", ""));

	return (result){passed, description};
}

result test_string_equals_StringWithOtherString() {
	int passed = 0;
	char* description = "string_equals(char*, char*) : StringWithOtherString CASE";

	passed = is_false(string_equals("abc", "efg"));

	return (result){passed, description};
}

result test_string_equals_StringWithSameString() {
	int passed = 0;
	char* description = "string_equals(char*, char*) : StringWithSameString CASE";

	passed = is_true(string_equals("abc", "abc"));

	return (result){passed, description};
}

////
// test int string_substring(char*, char*)

result test_string_substring_EmptyStrings() {
	int passed = 0;
	char* description = "string_substring(char*, char*) : 2EmptyString CASE";

	passed = is_true(string_substring("", ""));

	return (result){passed, description};
}

result test_string_substring_EmptyStringNULL() {
	int passed = 0;
	char* description = "string_substring(char*, char*) : EmptyStringNULL CASE";

	passed = is_false(string_substring("", 0));

	return (result){passed, description};
}

result test_string_substring_NULL() {
	int passed = 0;
	char* description = "string_substring(char*, char*) : NULL CASE";

	passed = is_false(string_substring(0, 0));

	return (result){passed, description};
}

result test_string_substring_StringWithOtherString() {
	int passed = 0;
	char* description = "string_substring(char*, char*) : StringWithOtherString CASE";

	passed = is_false(string_substring("abc", "efg"));

	return (result){passed, description};
}

result test_string_substring_StringWithEmptyString() {
	int passed = 0;
	char* description = "string_substring(char*, char*) : StringWithEmptyString CASE";

	passed = is_true(string_substring("abc", ""));

	return (result){passed, description};
}

result test_string_substring_StringWithSubstring() {
	int passed = 0;
	char* description = "string_substring(char*, char*) : StringWithSubstring CASE";

	passed = is_true(string_substring("abc", "ab"));

	return (result){passed, description};
}

result test_string_substring_StringWithBiggerString() {
	int passed = 0;
	char* description = "string_substring(char*, char*) : StringWithBiggerString CASE";

	passed = is_false(string_substring("abc", "efghi"));

	return (result){passed, description};
}

result test_string_substring_StringWithBiggerSuperstring() {
	int passed = 0;
	char* description = "string_substring(char*, char*) : StringWithBiggerSuperstring CASE";

	passed = is_false(string_substring("abc", "abcdefg"));

	return (result){passed, description};
}

////
// main function and test list definition

static tests TESTS = {
	// string_length(char*)
	&test_string_length_EmptyString,
	&test_string_length_NULL,
	&test_string_length_Len1,
	&test_string_length_Len5,

	// string_equals(char*, char*)
	&test_string_equals_EmptyStrings,
	&test_string_equals_EmptyStringNULL,
	&test_string_equals_NULL,
	&test_string_equals_StringWithEmpty,
	&test_string_equals_StringWithOtherString,
	&test_string_equals_StringWithSameString,

	// string_substring(char*, char*)
	&test_string_substring_EmptyStrings,
	&test_string_substring_EmptyStringNULL,
	&test_string_substring_NULL,
	&test_string_substring_StringWithOtherString,
	&test_string_substring_StringWithEmptyString,
	&test_string_substring_StringWithSubstring,
	&test_string_substring_StringWithEmptyString,
	&test_string_substring_StringWithBiggerString,
	&test_string_substring_StringWithBiggerSuperstring,
	0
};

int main(int argc, char** argv) {
	run_tests(TESTS);
	return 0;
}
