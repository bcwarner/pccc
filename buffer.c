
#include "buffer.h"
#include <pthread.h>

pccc_buffer*
pccc_buffer_init(char *name, char *contents, size_t len, unsigned int flags){
	// Allocate the memory for the buffer.
	pccc_buffer * ret = PCCC_MALLOC(pccc_buffer, 1);
	
	size_t namelen = strlen(name) + 1; // Plus one for null ptr.
	ret->name = PCCC_MALLOC(char, namelen);
	strncpy(ret->name, name, namelen);

	ret->contents = PCCC_MALLOC(char, len + 1);
	strncpy(ret->contents, contents, len + 1);

	ret->len = len;
	ret->flags = flags;

	// Create the mutex.

	ret->mutex = PCCC_MALLOC(pthread_mutex_t, 1);
	pthread_mutex_init(ret->mutex, NULL);
	return ret;
}

void
pccc_buffer_update(pccc_buffer *buf, char *contents, size_t len){
	// Lock the mutex.
	pthread_mutex_lock(buf->mutex);

	// Reallocate the data.
	buf->contents = realloc(buf->contents, len);

	// Move the contents in.
	strncpy(buf->contents, contents, len);

	// Unlock the mutex.
	pthread_mutex_unlock(buf->mutex);
}