/*
Defines the structure of a buffer
*/

#pragma once

#include <pthread.h>

#include "macros.h"
#include "types.h"


pccc_buffer*
pccc_buffer_init(char *name, char *contents, size_t len, unsigned int flags);

void
pccc_buffer_update(pccc_buffer *buf, char *contents, size_t len);