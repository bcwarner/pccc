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

char *
pccc_buffer_get_folder(char *name, size_t *length);