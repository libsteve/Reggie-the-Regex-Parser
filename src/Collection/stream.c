#include "Collection/stream.h"

// get a pointer to the next value in the stream
static void *stream_peek(struct stream s) {
	if (!s.closed(s)) {
		return s.data;
	}
	return NULL;
}

// advance the stream to the next value
// returns a stream object that will be able to peek to the next value
static struct stream stream_advance(struct stream s) {
	if (!s.closed(s)) {
		return stream(s.data + s.part_size, s.part_size, s.length - 1);
	}
	return stream(NULL, 0, 0);
}

// is there any more data in the stream?
// returns true if there is no more data in the stream
static bool stream_closed(struct stream s) {
	return (s.length == 0) || (s.data == NULL);
}

// create a stream instance using the default functions
struct stream stream(void *data, size_t part_size, size_t length) {
	return (struct stream){ .peek = stream_peek, 
							.advance = stream_advance, 
							.closed = stream_closed, 
							.data = data, 
							.part_size = part_size,
							.length = length};
}

// create a stream instance using the provided custom functions and data pointer
struct stream customStream(peek_fn peek, advance_fn advance, closed_fn closed, void *data) {
	return (struct stream){ .peek = peek, 
							.advance = advance, 
							.closed = closed, 
							.data = data, 
							.part_size = 0,
							.length = 0};
}
