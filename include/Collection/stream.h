#ifndef REGGIE_STREAM_H
#define REGGIE_STREAM_H

#include <stdlib.h>
#include <stdbool.h>
#include "strings.h"

// a structure to represent a stream of data
struct stream {
	// get a pointer to the next value in the stream
	void *(*peek)(struct stream s);

	// get the size of the peek() function
	size_t (*peek_size)(struct stream s);

	// advance the stream to the next value
	// returns a stream object that will be able to peek to the next value
	// there should be no side effects to this function
	struct stream (*advance)(struct stream s);

	// is there any more data in the stream?
	// returns true if there is no more data in the stream
	bool (*closed)(struct stream s);

	// a pointer to any arbitrary data
	// default functions assume this is an array
	const void *const data;

	// the size of each element in the data array
	const size_t part_size;

	// the total amount of elements in the data array
	const size_t length;
};

typedef void *(*peek_fn)(struct stream);
typedef size_t (*peek_size_fn)(struct stream);
typedef struct stream (*advance_fn)(struct stream);
typedef bool (*closed_fn)(struct stream);

// create a stream instance using the default functions
struct stream stream(void *data, size_t part_size, size_t length);

// create a stream instance using the provided custom functions and data pointer
// a NULL or 0 value to any field will provide the default functionality if desired
struct stream customStream(peek_fn peek, peek_size_fn peek_size, advance_fn advance, closed_fn closed, void *data, size_t part_size);

#define stringstream(string) (stream(string, sizeof(char), string_length(string)))

#endif
