/* 
    PCCC: A portable library for context-cognizant completions.
    Copyright (C) 2017  bcwarner

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "buffer.h"
#include <pthread.h>

pccc_buffer*
pccc_buffer_init(char *name, char *contents, size_t len, unsigned int flags){
	// Allocate the memory for the buffer.
	pccc_buffer * ret = PCCC_MALLOC(pccc_buffer, 1);
	
	size_t namelen = strlen(name) + 1; // Plus one for null ptr.
	ret->name = PCCC_MALLOC(char, namelen);
	strncpy(ret->name, name, namelen);

	if (!(flags & PCCC_BUFFER_PREALLOC)){
		ret->contents = PCCC_MALLOC(char, len + 1);
		strncpy(ret->contents, contents, len + 1);
	} else {
		ret->contents = contents;
	}

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

char *
pccc_buffer_get_folder(char *name, size_t *length){
	int len = strlen(name) - 1;
	
	while (len != 0 && name[len] != PCCC_FOLDER_PATH){
		len--;
		PCCC_PRINTF("Current char: %c\n", name[len]);
	}

	if (len == 0)
		return NULL;

	char *result = PCCC_MALLOC(char, len + 1);
	strncpy(result, name, len);
	result[len] = '\0';
	PCCC_PRINTF("Result of folder search: %s\n", result);

	if (length != NULL)
		*length = len;

	return result;
}

/*void
pccc_buffer_from_file(pccc_context *ctxt, char *path){

}*/