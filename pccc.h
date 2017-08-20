/*
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

void
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
