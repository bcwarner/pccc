/*
Defines the structure of a buffer
*/

#include <pthread.h>

#include "macro.h"

typedef struct pccc_buffer {
	char * contents;
	char * name;
	int len;
	int flags;
	pthread_mutex_t *mutex; // Used for locking the analysis of the buffer.
} pccc_buffer;

pccc_buffer*
pccc_buffer_init(char *name, char *contents, int len, int flags){
	// Allocate the memory for the buffer.

	// Allocate the string 

	// Create the mutex.
}

pccc_buffer_update(pccc_buffer *buf, char *contents, int len){
	// Lock the mutex.

	// Reallocate the data.

	// Move the contents in.

	// Unlock the mutex.
}