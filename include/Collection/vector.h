#ifndef REGGIE_VECTOR_H
#define REGGIE_VECTOR_H

#include <stdlib.h>

// a structure to represent a vector, which is a friendly interface to c arrays
typedef struct vector {
	// the backing array instance
	void *array;

	// the size of each element within the array
	size_t part_size;

	// the length of the array
	size_t length;
} *vector;

// iterate over the given vector of the given type
#define ITERATE(vec, type, it) \
	for (type *it = vec->array; \
		 it < vec->array + (vec->length * vec->part_size); \
		 it += vec->part_size)

// access the value held within the iterator
#define GET(it) (*it)

// create a new vector instance for elements of the given size
vector vector_create(size_t part_size);

// create a new vector instance with a copy of the given array and elements
vector vector_createArrayVector(const void *array, size_t part_size, size_t length);

// create a copy of the given vector instance
vector vector_copy(vector v);

// destroy the given vector instance
void vector_destroy(vector v);

// create a vector instance holding the given type
#define vector_createOf(type) (vector_create(sizeof(type)))

// create a vector instance with a copy of the given array holding the given type of the given length
#define vector_createOfArray(array, type, length) (vector_createArrayVector(array, sizeof(type), length))

// push an element to the end of the vector
// pass in a pointer to the element to push
vector vector_push(vector v, void *value);

// push an element to the beginning of the vector
// pass in a pointer to the element to push
vector vector_rpush(vector v, void *value);

// remove the last element from the vector
vector vector_pop(vector v);

// remove the first element from the vector
vector vector_rpop(vector v);

// get a pointer to the last element of the vector
void *vector_peek(vector v);

// get a pointer to the first element of the vector
void *vector_rpeek(vector v);

// get a pointer to the element at the given index of the vector
void *vector_get(vector v, size_t index);

// set the element at the given index to the value at the given pointer address
vector vector_set(vector v, size_t index, void *value);

// modify vector v by appending a copy of every element from v2 to the end of vector v
vector vector_append(vector v, vector v2);

// modify vector v by appending a copy of every element from v2 to the beginning of vector v
vector vector_rappend(vector v, vector v2);

// create a new vector instance that copies the data from the two given vectors
vector vector_appendCreate(vector v1, vector v2);

// push the value of the given type to the end of the vector
#define vector_pushOf(vect, type, value)  (vector_push(vect, (type []){value}))

// push the value of the given type to the beginning of the vector
#define vector_rpushOf(vect, type, value) (vector_rpush(vect, (type []){value}))

// get the last value of the given type in the vector
#define vector_peekOf(vect, type)  (*((type *)vector_peek(vect)))

// get the first value of the given type in the vector
#define vector_rpeekOf(vect, type) (*((type *)vector_rpeek(vect)))

// get the value of the given type at the given index
#define vector_getOf(vect, type, index) (*((type *)vector_get(vect, index)))

// get the value at the given index to the given value of the given type
#define vector_setOf(vect, type, index, value) (vector_set(vect, index, (type []){value}))

#endif
