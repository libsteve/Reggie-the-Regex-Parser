#include "Collection/vector.h"
#include <stdlib.h>
#include <string.h>

// create a new vector instance for elements of the given size
vector vector_create(size_t part_size) {
    return vector_createArrayVector(NULL, part_size, 0);
}

// create a new vector instance with a copy of the given array and elements
vector vector_createArrayVector(const void *array, size_t part_size, size_t length) {
    vector v = calloc(1, sizeof(struct vector));
    v->array = array ? ({
        void *copy = calloc(length, part_size);
        memcpy(copy, array, part_size * length);
        copy;
    }) : NULL;
    v->part_size = part_size;
    v->length = length;
    return v;
}

// create a copy of the given vector instance
vector vector_copy(vector v) {
    return vector_createArrayVector(v->array, v->part_size, v->length);
}

// destroy the given vector instance
void vector_destroy(vector v) {
    if (v->array) free(v->array);
    free(v);
}

// push an element to the end of the vector
// pass in a pointer to the element to push
vector vector_push(vector v, void *value) {
    if (v->array) {
        v->array = realloc(v->array, (v->length + 1) * v->part_size);
    } else {
        v->array = calloc(1, v->part_size);
    }
    memcpy(v->array + (v->length * v->part_size), value, v->part_size);
    v->length += 1;
    return v;
}

// push an element to the beginning of the vector
// pass in a pointer to the element to push
vector vector_rpush(vector v, void *value) {
    if (v->array) {
        v->array = realloc(v->array, (v->length + 1) * v->part_size);
        memmove(v->array + v->part_size, v->array, v->length * v->part_size);
    } else {
        v->array = calloc(1, v->part_size);
    }
    memcpy(v->array, value, v->part_size);
    v->length += 1;
    return v;
}

// remove the last element from the vector
vector vector_pop(vector v) {
    if (v->length > 0) {
        v->length -= 1;
        v->array = realloc(v->array, v->length * v->part_size);
    }
    return v;
}

// remove the first element from the vector
vector vector_rpop(vector v) {
    if (v->length > 0) {
        v->length -= 1;
        memmove(v->array, v->array + v->part_size, v->length * v->part_size);
        v->array = realloc(v->array, v->length * v->part_size);
    }
    return v;
}

// get a pointer to the last element of the vector
void *vector_peek(vector v) {
    return vector_get(v, v->length - 1);
}

// get a pointer to the first element of the vector
void *vector_rpeek(vector v) {
    return vector_get(v, 0);
}

// get a pointer to the element at the given index of the vector
void *vector_get(vector v, size_t index) {
    if (v->length > 0 && index >= 0 && index < v->length) {
        return v->array + (index * v->part_size);
    }
    return NULL;
}

// set the element at the given index to the value at the given pointer address
vector vector_set(vector v, size_t index, void *value) {
    if (v->length > 0 && index >= 0 && index < v->length) {
        memcpy(v->array + (index * v->part_size), value, v->part_size);
    }
    return v;
}

// PRIVATE
void *_new_array(vector v1, vector v2) {
    void *new = calloc(v1->length + v2->length, v1->part_size);
    if (v1->array) memcpy(new, v1->array, v1->length * v1->part_size);
    if (v2->array) memcpy(new + (v1->length * v1->part_size), v2->array, v2->length * v2->part_size);
    return new;
}

// modify vector v by appending a copy of every element from v2 to the end of vector v
vector vector_append(vector v, vector v2) {
    if (v->part_size == v2->part_size) {
        void *array = _new_array(v, v2);
        free(v->array);
        v->array = array;
        v->length += v2->length;
    }
    return v;
}

// modify vector v by appending a copy of every element from v2 to the beginning of vector v
vector vector_rappend(vector v, vector v2) {
    if (v->part_size == v2->part_size) {
        void *array = _new_array(v2, v);
        free(v->array);
        v->array = array;
        v->length += v2->length;
    }
    return v;
}

// create a new vector instance that copies the data from the two given vectors
vector vector_appendCreate(vector v1, vector v2) {
    vector v = vector_copy(v1);
    return vector_append(v, v2);
}
