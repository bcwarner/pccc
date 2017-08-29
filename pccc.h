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

API
Serves as an interface between the library and the main functions.
*/

#pragma once

#include "macros.h"
#include "types.h"


// Creates the context for the program. Takes an array of buffers.
pccc_context* 
pccc_init(pccc_buffer ** buf, int n);

// Analyzes a string. Returns a ST with all possible symbols.
pccc_suggestions*
pccc_suggest(pccc_context* ctxt, char *s);

// Analyzes a string without any preceding tokens. Returns a ST with all possible symbols.
pccc_suggestions*
pccc_suggest_prefix(pccc_context* ctxt, char *s);

// Adds a buffer to a context.
void
pccc_add_buffer(pccc_context *ctxt, pccc_buffer *buf);

pccc_buffer*
pccc_add_new_buffer(pccc_context *ctxt, char *name, char *contents, int len, int flags);

void 
pccc_update_buffer(pccc_context *ctxt, char *name, char *contents, int len);

// Gets a buffer from a context.
pccc_buffer* 
pccc_get_buffer(pccc_context *ctxt, char *name);

// Analyzes a buffer. Returns if it succeeded. Note that the API will pay attention to the address and whether it has already been analyzed as a buffer.
int
pccc_analyze(pccc_context* ctxt, pccc_buffer * buf);

// Analyzes all buffers. Returns if it succeeded. Note that the API will pay attention to the address and whether it has already been analyzed as a buffer.
int
pccc_analyze_all(pccc_context* ctxt);
